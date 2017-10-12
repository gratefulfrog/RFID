/*
 * common_uart.h
 *
 *  Created on: 2013-5-29
 *      Author: pasino
 */
 
#include <termios.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>


#define UINT8 		unsigned char

//璁剧疆鏁版嵁浣嶏紝鍋滄浣嶏紝鏍￠獙浣�
//@param fd 涓插彛鎵撳紑浣嶇疆
//@param databits 鏁版嵁浣�
//@param stopbis 鍋滄浣�
//@param parity 鏍￠獙浣� 'n','N' 鏃犳牎楠屼綅锛�'o','O' 濂囨牎楠岋紝'e','E' 鍋舵牎楠� 锛�'s','S'鏃犳牎楠�
//@param readTime 璇诲彇瓒呮椂鏃堕棿
static int set_uart_parity(int fd, int databits, int stopbits, int parity, int readTime)
{
	tcflush(fd, TCIOFLUSH);

	struct termios opt;
	tcgetattr(fd, &opt);
	opt.c_cflag &= ~CSIZE;

	switch(databits)/*璁剧疆鏁版嵁浣嶆暟*/
	{
	case 5:
		opt.c_cflag |= CS5;
		break;
	case 6:
		opt.c_cflag |= CS6;
		break;
	case 7:
		opt.c_cflag |= CS7;
		break;
	case 8:
		opt.c_cflag |= CS8;
		break;
	default:
		return -1;
	}

	switch (parity)
	{
		case 'n':
		case 'N':
		case 0:
			opt.c_cflag &= ~PARENB;   /* 鏃犳牎楠� */
			opt.c_iflag |= INPCK;     /* Disable parity checking */
			break;
		case 'o':
		case 'O':
		case 1:
			opt.c_cflag |= (PARODD | PARENB); /* 璁剧疆涓哄鏁堥獙*/
			opt.c_iflag |= INPCK;             /* Disable parity checking */
			break;
		case 'e':
		case 'E':
		case 2:
			opt.c_cflag |= PARENB;     /* 璁剧疆涓哄伓鏁堥獙*/
			opt.c_cflag &= ~PARODD;   /* 杞崲涓哄伓鏁堥獙*/
			opt.c_iflag |= INPCK;       /* Disnable parity checking */
			break;
		case 'S':
		case 's':  /*as no parity*/
			opt.c_cflag &= ~PARENB;
			break;
		default:
			//fprintf(stderr,"Unsupported parity\n");
			return -1;
	}

	/* 璁剧疆鍋滄浣�*/
	switch (stopbits)
	{
		case 1:
			opt.c_cflag &= ~CSTOPB;
			break;
		case 2:
			opt.c_cflag |= CSTOPB;
		   break;
		default:
			// fprintf(stderr,"Unsupported stop bits\n");
			 return -1;
	}
	opt.c_cc[VTIME] = readTime; /* 璁剧疆瓒呮椂0.1 seconds*/
	opt.c_cc[VMIN] = 0; /* define the minimum bytes data to be read */
	/**
	* 濡傛灉涓嶆槸寮�鍙戠粓绔箣绫荤殑锛屽彧鏄覆鍙ｄ紶杈撴暟鎹紝鑰屼笉闇�瑕佷覆鍙ｆ潵澶勭悊锛岄偅涔堜娇鐢ㄥ師濮嬫ā寮�(Raw Mode)鏂瑰紡鏉ラ�氳锛岃缃柟寮忓涓嬶細
	*/
	opt.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
	opt.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
	opt.c_oflag &= ~OPOST;

//	opt.c_cflag |= (CLOCAL | CREAD);
//	opt.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);  //Input
//	opt.c_oflag &= ~OPOST;   //Output
//	opt.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
//	opt.c_oflag &= ~(ONLCR | OCRNL);
//	opt.c_iflag &= ~(IXON | IXOFF | IXANY);

	if (tcsetattr(fd,TCSANOW,&opt) != 0)
	{
		perror("SetupSerial 3");
		return -1;
	}


	tcflush(fd, TCIOFLUSH);
	return 0;
}


//璁剧疆閫熺巼
//@param fd 涓插彛鎵撳紑鐨勫湴鍧�
//@param speed 涓插彛閫熺巼
static int set_uart_speed(int fd, int speed)
{
	struct termios opt;
	tcflush(fd, TCIOFLUSH);
	tcgetattr(fd, &opt);
	cfsetispeed(&opt,speed);
	cfsetospeed(&opt,speed);
	int status = tcsetattr(fd,TCSANOW,&opt);
	tcflush(fd,TCOFLUSH);
	return status;
}

//鎵撳紑涓插彛
//@param dev 璁惧鍦板潃
//@param speed 閫熺巼
//@param databits 鏁版嵁浣�
//@param stopbis 鍋滄浣�
//@param parity 鏍￠獙浣� 'n','N' 鏃犳牎楠屼綅锛�'o','O' 濂囨牎楠岋紝'e','E' 鍋舵牎楠� 锛�'s','S'鏃犳牎楠�
//@param readTime 璇诲彇瓒呮椂鏃堕棿
static int uart_open(char* dev, int speed, int databits, int stopbis,int parity,int readTime)
{
	int fd;
	fd = open(dev, O_RDWR | O_NOCTTY | O_NDELAY, S_IRWXO);
	if (fd < 0){
//		logd("open ttyS2 devices failed: errno=%d(%s)", errno, strerror(errno));
		printf("open ttyUSB0 devices failed: errno=%d(%s)\n", errno, strerror(errno));
		return -1;
	}
	if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0){
//		loge("fcntl setfl failed: errno=%d(%s)", errno, strerror(errno));
		printf("fcntl setfl failed: errno=%d(%s)", errno, strerror(errno));
		goto uart_open_close;
	}
	int ret = set_uart_speed(fd, speed);
	if(ret == -1)
	{
		goto uart_open_close;
	}
	ret = set_uart_parity(fd, databits, stopbis, parity,readTime);
	if(ret == -1)
	{
		goto uart_open_close;
	}
	return fd;
	uart_open_close:
		close(fd);
//	loge("error open UART");
	printf("error open UART");
	return -1;
}




