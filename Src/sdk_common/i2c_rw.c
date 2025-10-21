#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/fcntl.h>
#include <sys/ioctl.h>

#define BB_I2C_UNKNOW       0
#define BB_I2C_READ         1
#define BB_I2C_WRITE        2
#define BB_I2C_MSGREAD      3		/* 目前用于I2C Sensor 的访问，16位的regaddr及16位的data*/
#define BB_I2C_MSGWRITE		4	    /* 目前用于I2C Sensor 的访问，16位的regaddr及16位的data*/
#define BB_I2C_SENSOR       5       
#define BB_I2C_OVREAD       6		/* 目前用于I2C OV 10633 Sensor 的访问，16位的regaddr及8位的data*/
#define BB_I2C_OVWRITE		7	    /* 目前用于I2C OV 10633 Sensor 的访问，16位的regaddr及8位的data*/

#define BB_I2C_READ_ARGC    5
#define BB_I2C_WRITE_ARGC   6

#define BB_I2C_READ_MAX     64

enum
{
    DHC_I2C_8BITS = 0,
    DHC_I2C_16BITS,
    DHC_I2C_A16D8
};

#define LOWORD(l)           ((unsigned short)(l))
#define HIWORD(l)           ((unsigned short)(((unsigned int)(l) >> 16) & 0xFFFF))

#define LOBYTE(w)           ((unsigned char)(w))
#define HIBYTE(w)           ((unsigned char)(((unsigned short)(w) >> 8) & 0xFF))


void help();

static int i2c_handle = 0;
static char *dev_array[] = 
{
	"/dev/i2c-0",
	"/dev/i2c-1",
	"/dev/i2c-2",
	"/dev/i2c-3",
	"/dev/i2c-4",
	"/dev/i2c-5",
};

/*************************************************
Function: ParseHexUL
Description:将输入的字符串转换成十六进制数据,私有函数
Input:  *s      ==> 字符串指针
Output: *number ==> 十六进制数据
Return: 返回转换的有效数字位数
History:
*************************************************/
static unsigned char ParseHexUL( char *s, int *number)
{
    unsigned char   bits  = 0;
    int             val = 0;

    while ( *s != '\0')
    {
        if ((*s >= '0') && (*s <= '9'))
            val = val * 16 + *s++ - '0';
        else if ((*s >= 'a') && (*s <= 'f'))
            val = val * 16 + *s++ - 'a' + 10;
        else if ((*s >= 'A') && (*s <= 'F'))
            val = val * 16 + *s++ - 'A' + 10;
        else
            break;

        if (++bits >= 8)
            break;      /* 8 Hex bits Max: tU4 Para */
    }

    *number = val;

    return (bits);

}

/*************************************************
Function: avi_CommStr2Int
Description:将输入的字符串转换成十六进制数据或者十进制数据,外部调用接口
Input:  *str      ==> 字符串指针
Output: 无
Return: 返回转换结果
*************************************************/
int str2int(char *str)
{
    char * p = (char*)str;
    int number;
    
    if ((p[0] == '0') && (p[1] == 'x'))
    {
        p += 2;
        ParseHexUL(p, &number);
    }
    else
    {
        number = (unsigned int)atoi(p);
    }
    
    return number;
}

static void DHC_TEST_I2C_MsgRead( int dev_addr, int addr, unsigned char *pbuf, int count, int mode )
{
	unsigned char i2cbuf[ BB_I2C_READ_MAX ], ad[ 2 ];
	struct i2c_msg msgs[ 2 ];
	struct i2c_rdwr_ioctl_data rw_msg;
	
	if ( count > BB_I2C_READ_MAX )
	{
		count = BB_I2C_READ_MAX;
	}
	else if ( 0 == count )
	{
	    if ( DHC_I2C_16BITS == mode )
        {
		    count = 2;
		}
        else
        {
            count = 1;
        }
	}
	
    /* packet the message0 */
    msgs[ 0 ].addr = (unsigned short)( dev_addr ) >> 1;
    msgs[ 0 ].flags = 0;
    
    //set reg address
    if ( DHC_I2C_8BITS == mode )
    {
        ad[ 0 ] = LOBYTE( addr );
        msgs[ 0 ].len = 1;
    }
    else
    {
        ad[ 0 ] = HIBYTE( addr );
	    ad[ 1 ] = LOBYTE( addr );
        msgs[ 0 ].len = 2;
    }
    
    msgs[ 0 ].buf = ad;
    
    /* packet the message1 */
    msgs[ 1 ].addr = (unsigned short)( dev_addr ) >> 1;
    msgs[ 1 ].flags = I2C_M_RD;
    msgs[ 1 ].len = count;
    msgs[ 1 ].buf = i2cbuf;
    
    rw_msg.msgs = msgs;
    rw_msg.nmsgs = 2;
    
    ioctl( i2c_handle, I2C_RDWR, &rw_msg );
    
    memcpy( pbuf, i2cbuf, count);
}


static void DHC_TEST_I2C_MsgWrite( int dev_addr, int addr, int value, int mode )
{
	unsigned char ad[ 4 ];
	struct i2c_msg msgs[ 1 ];
	struct i2c_rdwr_ioctl_data rw_msg;

	//printf("Write: dev=%#x, addr=%#x, val=%#x\n", dev_addr, addr, value);
	
	/* packet the message0 */
	msgs[ 0 ].addr = (unsigned short)( dev_addr ) >> 1;
	msgs[ 0 ].flags = 0;
	
	if ( DHC_I2C_8BITS == mode )
    {
    	ad[ 0 ] = LOBYTE(addr);
    	ad[ 1 ] = LOBYTE(value);
    	
    	msgs[ 0 ].len = 2;
    }
    else
    {
        
    	ad[ 0 ] = HIBYTE(addr);
    	ad[ 1 ] = LOBYTE(addr);
    	
    	if ( DHC_I2C_16BITS == mode )
        {
    	    ad[ 2 ] = HIBYTE(value);
    	    ad[ 3 ] = LOBYTE(value);
    	
    	    msgs[ 0 ].len = 4;
        }
        else
        {
    	    ad[ 2 ] = LOBYTE(value);
    	
    	    msgs[ 0 ].len = 3;            
        }
        
    }
	
	msgs[ 0 ].buf = ad;
    
	rw_msg.msgs = msgs;
	rw_msg.nmsgs = 1;

	ioctl( i2c_handle, I2C_RDWR, &rw_msg );
}


void DHC_TEST_I2C_Read8B( unsigned int u32DevAddr, unsigned int u32RegAddr, unsigned char *pu8RevBuf, unsigned int u32Count )
{
    if ( i2c_handle > 0 )
    {
        DHC_TEST_I2C_MsgRead( u32DevAddr, u32RegAddr, pu8RevBuf, u32Count, DHC_I2C_8BITS );
    }
}

void DHC_TEST_I2C_Write8B( unsigned int u32DevAddr, unsigned int u32RegAddr, unsigned int u32Value )
{
    if ( i2c_handle > 0 )
    {
        DHC_TEST_I2C_MsgWrite( u32DevAddr, u32RegAddr, u32Value, DHC_I2C_8BITS );
    }    
}

void DHC_TEST_I2C_Read16B( unsigned int u32DevAddr, unsigned int u32RegAddr, unsigned char *pu8RevBuf, unsigned int u32Count )
{
    if ( i2c_handle > 0 )
    {
        DHC_TEST_I2C_MsgRead( u32DevAddr, u32RegAddr, pu8RevBuf, u32Count, DHC_I2C_16BITS );
    }    
}


void DHC_TEST_I2C_Write16B( unsigned int u32DevAddr, unsigned int u32RegAddr, unsigned int u32Value )
{
    if ( i2c_handle > 0 )
    {
        DHC_TEST_I2C_MsgWrite( u32DevAddr, u32RegAddr, u32Value, DHC_I2C_16BITS );
    }    
}


void DHC_TEST_I2C_WriteA16D8( unsigned int u32DevAddr, unsigned int u32RegAddr, unsigned int u32Value )
{
    if ( i2c_handle > 0 )
    {
        DHC_TEST_I2C_MsgWrite( u32DevAddr, u32RegAddr, u32Value, DHC_I2C_A16D8 );
    }    
}

int DHC_TEST_I2C_OpenDev(int num)
{
	char *pStrDevName = NULL;
	switch(	num )
	{
		case 0:
		pStrDevName = dev_array[0];
		break;
		case 1:
		pStrDevName = dev_array[1];
		break;
		case 2:
		pStrDevName = dev_array[2];
		break;
		case 3:
		pStrDevName = dev_array[3];
		break;
		case 4:
		pStrDevName = dev_array[4];
		break;
		case 5:
		pStrDevName = dev_array[5];
		break;
		default:
		break;
	}
	if(pStrDevName == NULL)
	{
		printf("selcet i2c controler %d bigger than the max hardware number 5\n",num);
		return -1;
	}
	i2c_handle = open( pStrDevName, 2 );
	
	if ( i2c_handle < 0 )
    {
         printf("Open %s device failed \n", pStrDevName );
		 return -1;
    }
	return 0;
}


void DHC_TEST_I2C_CloseDev( void )
{
    if ( i2c_handle > 0  )
    {
        close( i2c_handle );
    }
}

void help(void)
{
    printf("i2c help:\n");
    printf("Read by 8Bits   : i2c r devaddr regaddr[8Bits] count i2cIdex[0-5]\n");
    printf("Read by 8Bits   : i2c i devaddr regaddr[16Bits] count i2cIdex[0-5]\n");
    printf("Read by 16Bits  : i2c g devaddr regaddr[16Bits] count i2cIdex[0-5]\n");
    printf("Write by 8Bits  : i2c w devaddr regaddr[8Bits] value[8Bits] i2cIdex[0-5]\n");
    printf("Write by 8Bits  : i2c o devaddr regaddr[16Bits] value[8Bits] i2cIdex[0-5]\n");
    printf("Write by 16Bits : i2c p devaddr regaddr[16Bits] value[16Bits] i2cIdex[0-5]\n");
}
#if 0
int main(int argc, char *argv[])
{
	unsigned int ops;
	int dev_addr, addr,cnt,i2cNum,ret;
	int tmp;
	unsigned char rbuf[ BB_I2C_READ_MAX * 2 ];   /* *2 for 16bits */
	
	if(argc != 6 )
	{
	    printf("err argc num!\n");
		help();
		return 0;
	}
	
	i2cNum = ( int )str2int( argv[argc - 1] );
	ret = DHC_TEST_I2C_OpenDev(i2cNum);
    if(ret != 0)
	{
		help();
		return 0;
	}
	/* check the argv */
	ops = BB_I2C_UNKNOW;
	if ( argc > 1 )
	{
		if ( 'r' == argv[1][0] )
		{
			ops = BB_I2C_READ;
		}
		else if ( 'w' == argv[1][0] )
		{
			ops = BB_I2C_WRITE;
		}
		else if ( 'g' == argv[1][0] )
		{
			ops = BB_I2C_MSGREAD;
		}
		else if ( 'p' == argv[1][0] )
		{
			ops = BB_I2C_MSGWRITE;
		}
		else if ( 'i' == argv[1][0] )
		{
			ops = BB_I2C_OVREAD;
		}
		else if ( 'o' == argv[1][0] )
		{
			ops = BB_I2C_OVWRITE;
		}
		else if ( 's' == argv[1][0] )
        {
            ops = BB_I2C_SENSOR;
        }
		else
		{
			goto _i2c_out;
		}
	}
    
	/* do for read or write */
	switch ( ops )
	{
		case BB_I2C_READ:
		case BB_I2C_MSGREAD:
        case BB_I2C_OVREAD:
		{
			if ( argc < BB_I2C_READ_ARGC  )
			{
			    printf("err input format!\n");
				help();
			}
			else
			{
				dev_addr = ( int )str2int( argv[2] );
				addr = (int)str2int( argv[3] );
				cnt = 0;

				if ( argc > 4 )
				{
					cnt = (int)str2int( argv[4] );
				}

				if ( cnt > BB_I2C_READ_MAX )
				{
					cnt = BB_I2C_READ_MAX;
				}
				else if ( 0 == cnt )
				{
					cnt = 1;
				}

				if ( BB_I2C_READ == ops )
				{
					DHC_TEST_I2C_Read8B( dev_addr, addr, rbuf, cnt );
					for ( tmp = 0; tmp < cnt; tmp ++ )
					{
						printf( "I2CR[%#x]->[%#x] = %#x\n", dev_addr, addr + tmp, rbuf[tmp] );
					}
				}
				else if ( BB_I2C_MSGREAD == ops )
				{
					DHC_TEST_I2C_Read16B( dev_addr, addr, rbuf, cnt * 2 );
					for ( tmp = 0; tmp < cnt; tmp ++ )
					{
						printf( "I2CR[%#x]->[%#x] = %#x\n", dev_addr, addr + tmp * 2,((rbuf[2 * tmp]) << 8) + rbuf[2 * tmp + 1] );
					}
				}
				else
                {
                    /* for 9931*/                    
					for ( tmp = 0; tmp < cnt; tmp ++ )
					{
						DHC_TEST_I2C_Read16B( dev_addr, addr+tmp, rbuf, 1 );
						printf( "I2CR[%#x]->[%#x] = %#x\n", dev_addr, addr + tmp, rbuf[0] );
					}
                }
			}

			break;
		}
		case BB_I2C_WRITE:
		case BB_I2C_MSGWRITE:
        case BB_I2C_OVWRITE:
		{
			if ( BB_I2C_WRITE_ARGC != argc )
			{
			    printf("err input format!\n");
				help();
			}
			else
			{
				dev_addr = (int)str2int( argv[2] );
				addr = (int)str2int( argv[3] );
				tmp = (int)str2int( argv[4] );

				if ( BB_I2C_WRITE == ops )
				{
					DHC_TEST_I2C_Write8B( dev_addr, addr, tmp );
					printf( "EI2CW[%#x]->[%#x] = %#x\n", dev_addr, addr, tmp );
				}
				else if ( BB_I2C_MSGWRITE == ops )
				{
					DHC_TEST_I2C_Write16B( dev_addr, addr, tmp );
					printf( "TI2CW[%#x]->[%#x] = %#x\n", dev_addr, addr, tmp );                    	
				}
				else
                {
                    DHC_TEST_I2C_WriteA16D8( dev_addr, addr, tmp );
                    printf( "OI2CW[%#x]->[%#x] = %#x\n", dev_addr, addr, tmp );   
                }
			}
			break;
		}
		
		case BB_I2C_SENSOR:
        {
            break;
        }
		default:
		{
			printf("unsupport this command[%s]\n", argv[1] );
			goto _i2c_out;
		}
	}

_i2c_out:
    DHC_TEST_I2C_CloseDev();
	return 0;
}
#endif
