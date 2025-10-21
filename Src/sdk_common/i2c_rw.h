
void DHC_TEST_I2C_Read8B( unsigned int u32DevAddr, unsigned int u32RegAddr, unsigned char *pu8RevBuf, unsigned int u32Count );
void DHC_TEST_I2C_Write8B( unsigned int u32DevAddr, unsigned int u32RegAddr, unsigned int u32Value );
void DHC_TEST_I2C_Read16B( unsigned int u32DevAddr, unsigned int u32RegAddr, unsigned char *pu8RevBuf, unsigned int u32Count );
void DHC_TEST_I2C_Write16B( unsigned int u32DevAddr, unsigned int u32RegAddr, unsigned int u32Value );
void DHC_TEST_I2C_WriteA16D8( unsigned int u32DevAddr, unsigned int u32RegAddr, unsigned int u32Value );
void help(void);

int DHC_TEST_I2C_OpenDev(int num);
void DHC_TEST_I2C_CloseDev( void );
