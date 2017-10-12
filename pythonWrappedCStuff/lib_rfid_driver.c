#include "lib_rfid_driver.h"

int HeadIdx=0,TailIdx=0;
int uhf_uart_fd = -1;


int bytes2HexString(char *src, char *des, int size)
{
	int resval		= -1;
	int i;
	char 	hdata, ldata;

	if ((NULL == src) || (NULL == des))
	{
		return resval;
	}

	for (i=0; i<size; i++)
	{

		hdata = src[i] >> 4;
		ldata = src[i] & 0x0F;

		if (hdata < 10)
		{
			des[2*i]	= hdata + '0';
		}
		else
		{
			des[2*i] 	= (hdata-10) + 'A';
		}

		if (ldata < 10)
		{
			des[2*i + 1]	= ldata + '0';
		}
		else
		{
			des[2*i + 1] 	= (ldata-10) + 'A';
		}

		des[2*i + 2]		= '\0';
		resval  = 0;

	}


	return resval;
}

int receive_serial_bytes(unsigned char *pszBuffer, int iLength, int uart_fd)
{
	int iRes = 0;

	//printf("%s\n", __FUNCTION__);

	if(uart_fd == -1)
	{
		return 0;
	}

	//
	iRes = read(uart_fd, pszBuffer, iLength);

	//printf("iRes= %d\n", iRes);
/*
	int i = 0;
	for(i = 0; i < iLength; i++)
	{
		printf("%02x", *(pszBuffer+i));
	}
*/
	if (iRes == -1)//failure
	{
		return 0;
	}
	else//success
	{
		return iRes;
	}
	//printf("iRes= %d\n", iRes);
}

int send_serial_bytes(unsigned char *pszData, int iLength, int uart_fd)
{
	int iRes = 0;

	//printf("%s\n", __FUNCTION__);

	if(uart_fd == -1)
	{
		return 0;
	}

	//发送数据 send data
	iRes = write(uart_fd, pszData, iLength * sizeof(char));
	if (iRes == -1)//failure
	{
		return 0;
	}
	else//success
	{
		return iRes;
	}

}

unsigned char RecvUhfUartData(unsigned char *rbuf,unsigned char *rlen)
{
	unsigned char buf[250],len;
	unsigned char UhfUartBuf[1020];
	unsigned char TempBuf[1000];
	int i,j;
	unsigned char recvFlag = 0;
	unsigned long StartTime;
	int SearchIdx,SearchLen;
	int iRes;


	//printf("%s\n", __FUNCTION__);

	if(TailIdx == HeadIdx)
	{
		//StartTime = GetTickCount();		// record the start time
		for(i=0;i<500;i++)
		{
			iRes = receive_serial_bytes(buf,250,uhf_uart_fd);
			if (iRes > 0)
			{
				recvFlag = 1;
				if(iRes>250)
				{
					iRes = 250;
				}

				for(i=0;i<iRes;i++)
				{
					TailIdx = (TailIdx+1)%1000;
					UhfUartBuf[TailIdx] = buf[i];
				}
				iRes = 0;
			}
			else
			{
				if (recvFlag == 1)
				{
					//printf("recvflag == 1\n");
					recvFlag = 0;
					break;
				}
			}
			usleep(10000);
		}
	}
	if(TailIdx > HeadIdx)
	{
		SearchLen = TailIdx-HeadIdx;

		for(i=HeadIdx;i<HeadIdx+SearchLen;i++)
		{
			//printf("%d\n", SearchLen);
			//printf("%02x %02x\n", UhfUartBuf[i], UhfUartBuf[i+1]);
			if((UhfUartBuf[i]==0xA5) && (UhfUartBuf[i+1]==0x5A))
			{

				*rlen = UhfUartBuf[i+3];
				//printf("%d\n", *rlen);
				if(*rlen>SearchLen)
				{
					HeadIdx = TailIdx;
					//printf(" TailIdx > HeadIdx if  rbuf = %s\n", rbuf);
					return 0;
				}
				memcpy(rbuf,UhfUartBuf+i,*rlen);
				HeadIdx += *rlen;

				return 1;
			}
		}
	}
	else
	{
		if(TailIdx < HeadIdx)
		{
			memcpy(TempBuf,UhfUartBuf+HeadIdx,1000-HeadIdx);
			memcpy(TempBuf+1000-HeadIdx,UhfUartBuf,TailIdx);
			SearchLen = 1000-HeadIdx+ TailIdx;
			for(i=0;i<SearchLen;i++)
			{
				if((TempBuf[i]==0xA5) && (TempBuf[i+1]==0x5A))
				{
					*rlen = TempBuf[i+3];
					if(*rlen>SearchLen)
					{
						HeadIdx = TailIdx;
						//printf(" TailIdx < HeadIdx  if  rbuf = %s\n", rbuf);
						return 0;
					}
					memcpy(rbuf,TempBuf+i,*rlen);
					HeadIdx = (HeadIdx + *rlen)%1000;
					//printf(" TailIdx < HeadIdx  else   rbuf = %s\n", rbuf);

					return 1;
				}
			}
		}
		HeadIdx = TailIdx;
	}
	//printf("  end  rbuf = %s\n", rbuf);
	return 0;
}


unsigned char SendCommand_R2000(unsigned char* SendCommand, unsigned short SendNum)
{
	int iRes;


	//printf("%s\n", __FUNCTION__);


	iRes = send_serial_bytes(SendCommand, SendNum, uhf_uart_fd);
	if(iRes == 0)
		return 0;
	else
		return 1;
}

void clearRxFifo()
{
	tcflush(uhf_uart_fd, TCIFLUSH);
}

void clearTxFifo()
{
	tcflush(uhf_uart_fd, TCOFLUSH);
}
void UhfUartDataRst()
{
	TailIdx = HeadIdx =0;
	clearRxFifo();
}

int Um7_SendAndRecv(unsigned char cmd,
		    unsigned char *sendbuf,
		    unsigned char sendlen,
		    unsigned char *recvbuf,
		    unsigned char *recvlen)
{
	unsigned char sbuf[250],slen;
	unsigned char rbuf[250],rlen;
	unsigned short len;
	unsigned char idx = 0,i;
	unsigned char crcValue=0;

	//printf("%s\n", __FUNCTION__);

	UhfUartDataRst();

	sbuf[idx++] = 0xA5;
	sbuf[idx++] = 0x5A;
	sbuf[idx++] = 0;
	sbuf[idx++] = 8 + sendlen;
	sbuf[idx++] = cmd;
	if((sendbuf!=NULL) && (sendlen !=0))
	{
		memcpy(sbuf+idx,sendbuf,sendlen);
		idx += sendlen;
	}
	for(i=2;i<idx;i++)
		crcValue ^= sbuf[i];
	sbuf[idx++] = crcValue;
	sbuf[idx++] = 0x0D;
	sbuf[idx++] = 0x0A;

	clearRxFifo();

	len = (unsigned short)idx;

	if(SendCommand_R2000(sbuf,idx))
	{
//		for(i=0;i<idx;i++)
//		{
//			printf("sbuf sbuf[%d] = %x",i,sbuf[i]);
//		}
		if(RecvUhfUartData(rbuf, &rlen))
		{
			//printf("come here now ...!\n");
			//printf("%d\n", rlen);
			//printf("%02x %02x\n", rbuf[0], rbuf[1]);
			if((rbuf[0]==0xA5) && (rbuf[1]==0x5A))
			{
//				for(i=0;i<rlen;i++)
//				{
//					printf("NEW rbuf[%d] = %x",i,rbuf[i]);
//				}
				//printf("%02x\n", cmd);
				if(rbuf[4] != cmd+1)
				{
					return -1;
				}
				crcValue = 0;
				idx = rbuf[3];
				for(i=2;i<idx-3;i++)
					crcValue ^= rbuf[i];
				if(rbuf[idx-3] == crcValue)
				{
					if((recvbuf!=NULL) && (recvlen!=NULL))
					{
						*recvlen = rbuf[3]-8;
						memcpy(recvbuf,rbuf+5,rbuf[3]-8);
					}
					return 0;
				}
				else
				{
					return -2;
				}
			}
			else
			{
				return -3;
			}
		}
		else
			return -4;
	}
	else
		return -5;
}

/**********************************************************************
*	获取固件版本号 Get the firmware version number
*
**********************************************************************/
int GetUm7Fw(unsigned char *version){
  int Status;
  unsigned char  recvlen;
  
  Status = Um7_SendAndRecv(2, NULL, 0, version, &recvlen);
  //printf("receive length : %u\n",recvlen);
  return recvlen;
}
/*
	//printf("Status = %d\n", Status);
	if(Status != 0)
	{
		return -1;
	}
	else
	{
		/*
		loge("enter if Status equal zero ...!");
		int i = 0;
		for(i=0; i < recvlen; i++)
		{
			loge("%02x", recvBuf[i]);
		}
		
//		loge("%s....", recvBuf);

		if (bytes2HexString(recvBuf, newbuf, recvlen) == 0)
		{
			
		}
		else
		{
			return ERR_FW;
		}
		
		for(i=0; i < recvlen*2; i++)
		{
			loge("%02x", newbuf[i]);
		}
		 return 0;
	}
}
*/

/**********************************************************************
*获取硬件版本号 Get the hardware version number
*
**********************************************************************/
int GetUm7Hw(unsigned char *version){
  int Status;
  unsigned char recvlen;

  Status = Um7_SendAndRecv(0, NULL, 0, version, &recvlen);
  //printf("receive length : %u\n",recvlen);
  return recvlen;
}
/*	//printf("Status = %d\n", Status);
	if(Status != 0)
	{
		return -1;
	}
	else
	{
		/*
		printf("enter if Status equal zero ...!");
		int i = 0;
		for(i=0; i < recvlen; i++)
		{
			printf("%02x", recvBuf[i]);
		}

//		printf("%s....", recvBuf);
		if (bytes2HexString(recvBuf, newbuf, recvlen) == 0)
		{
			
		}
		else
		{
			return ERR_HW;
		}
		
		for(i=0; i < recvlen*2; i++)
		{
			//printf("%02x", newbuf[i]);
		}
		 return 0;
	}

}
*/


/**********************************************************************
*	获取 obtain RSSI
*
**********************************************************************/
int GetUm7Rssi(unsigned char * sum){
  int Status;
  unsigned char recvlen;
  
  Status = Um7_SendAndRecv(0x64, NULL, 0, sum, &recvlen);
  
  return recvlen;
}
	/*
	//printf("Status = %d", Status);
	if(Status != 0)
	{
		return -1;
	}
	else
	{

		printf("enter if Status equal zero ...!");
		int i = 0;
		for(i=0; i < recvlen; i++)
		{
			printf("%02x", recvBuf[i]);
		}

//		printf("%s....", recvBuf);
		if (bytes2HexString(recvBuf, newbuf, recvlen) == 0)
		{
			return (*env)->NewStringUTF(env, newbuf);
		}
		else
		{
			return (*env)->NewStringUTF(env, "未识别出固件版本号！ The firmware version number is not recognized");
		}

		for(i=0; i < recvlen*2; i++)
		{
			printf("%02x", newbuf[i]);
		}

	}
	return 0;
}
*/
/**********************************************************************
*	连续读取标签:  Read the label continuously
**********************************************************************/
int readmore(unsigned char* epc){
  int Status;
  unsigned char newbuf[50], recvlen;
  
  Status = Um7_SendAndRecv(0x82, NULL, 0, epc, &recvlen);
  if (Status !=0){
    return 0;
  }
  else{
    return recvlen;
  }
}

int readerID(unsigned char* epc){
  int Status;
  unsigned char newbuf[50], recvlen;
  
  Status = Um7_SendAndRecv(0x04, NULL, 0, epc, &recvlen);
  if (Status !=0){
    return 0;
  }
  else{
    return recvlen;
  }
}

int readerTemp(unsigned char* epc,float *tempOut){
  int Status;
  unsigned char newbuf[50], recvlen;
  
  Status = Um7_SendAndRecv(0x34, NULL, 0, epc, &recvlen);
  if (Status !=0){
    return 0;
  }
  else{
    short tmp = (short)(epc[1] * 256 + epc[2]);
    float temperature = (float)tmp;
    *tempOut = (float)(temperature / 100.0);
    return recvlen;
  }
}


int readone(unsigned char* epc){
  int Status;
  unsigned char newbuf[50], recvlen;
  
  Status = Um7_SendAndRecv(0x90, NULL, 0, epc, &recvlen);
  if (Status !=0){
    return 0;
  }
  else{
    return recvlen;
  }
}


/*
	//printf("Status = %d", Status);
/*	
	int i = 0;
	for(i=0; i < recvlen; i++)
	{
		loge("%02x", recvBuf[i]);
	}

	if(Status != 0)
	{
		return -1;
	}
	else
	{
//		loge("%s....", recvBuf);
		if (bytes2HexString(epc, newbuf, recvlen) == 0)
		{
			return 0;
		}
		else
		{
			return -1;
		}
	}
}
*/

/**********************************************************************
*	停止连续读取标签 Stop reading the label continuously
**********************************************************************/
int stopReadmore(){
  int Status;
  unsigned char recvBuf[500], newbuf[500], recvlen;
  
  Status = Um7_SendAndRecv(0x8C, NULL, 0, recvBuf, &recvlen);
  if (Status !=0){
    return 0;
  }
  else{
    return recvlen;
  }
}
/*	//printf("Status = %d", Status);
	if(Status != 0)
	{
		return -1;
	}
	else
	{
//		loge("%s....", recvBuf);
		if (bytes2HexString(recvBuf, newbuf, recvlen) == 0)
		{
			return 0;
		}
		else
		{
			return -1;
		}
	}

}
*/
/**********************************************************************
*	获取发射功率 Get transmit power
**********************************************************************/
int GetTxPower(unsigned char *sum){
	int Status;
	unsigned char recvBuf[50], newbuf[50], recvlen;

	Status = Um7_SendAndRecv(0x12, NULL, 0, sum, &recvlen);
	//printf("receive length : %u\n",recvlen);
	return recvlen;
	//printf("Status = %d", Status);
/*
	int i = 0;
	for(i = 0; i < recvlen; i++)
	{
		loge("recvBuf[%d] = %02x", i, recvBuf[i]);
	}

	if(Status != 0)
	{
		return -1;
	}
	else
	{
	  if (bytes2HexString(sum, newbuf, recvlen) == 0)  // src, des, size
		{

			int i = 0;
			for(i=0; i<recvlen*2;i++)
			{
				loge("newbuf[%d] = %02x", i, newbuf[i]);
			}
			return 0;
		}
		else
		{
			return -1;
		}
	}
*/

}
