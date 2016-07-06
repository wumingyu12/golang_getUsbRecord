波特率 115200 , 8N1地址范围 0 - 256KB (0 - 0x3FFFF)每次读写长度最多 2048 Byte1. 读{									0    1    2     3     4     5     6    7    8    9	 PC发送: 		0xA5 0x5A Addr3 Addr2 Addr1 Addr0 0x00 LenH LenL CS	 回复:   		0xA5 0x5A Addr3 Addr2 Addr1 Addr0 0x80 LenH LenL Data CS	 回复错误:  0xA5 0x5A Addr3 Addr2 Addr1 Addr0 0xC0 ErrCode CS	} 2. 写{							0    1		2			3			4			5			6		 7    8    9    9+dlen	 PC发送: 		0xA5 0x5A Addr3 Addr2 Addr1 Addr0 0x01 LenH LenL Data CS	 回复: 		  0xA5 0x5A Addr3 Addr2 Addr1 Addr0 0x81 LenH LenL CS		 回复错误:  0xA5 0x5A Addr3 Addr2 Addr1 Addr0 0xC1 ErrCode CS	}3. 错误代码{	  1  -  地址超范围	  2  -  长度超范围}4. 例{		//写		S: A5 5A 00 00 00 00 01 00 10 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 C2		R: A5 5A 00 00 00 00 81 00 10 90 					S: A5 5A 00 01 00 00 01 00 10 11 22 33 44 55 66 77 88 99 00 21 22 23 24 25 26 E3		R: A5 5A 00 01 00 00 81 00 10 91 				S: A5 5A 00 02 00 00 01 00 10 21 22 23 24 25 26 27 28 29 20 21 22 23 24 25 26 54		R: A5 5A 00 02 00 00 81 00 10 92 					S: A5 5A 00 03 00 00 01 00 10 31 32 33 34 35 36 37 38 39 30 31 32 33 34 35 36 55		R: A5 5A 00 03 00 00 81 00 10 93 				S: A5 5A 00 00 FF F5 01 00 10 AA BB CC DD EE FF 11 22 33 44 55 66 77 88 99 00 FC		R: A5 5A 00 00 FF F5 81 00 10 84 				//读		S: A5 5A 00 00 00 00 00 00 10 0F		R: A5 5A 00 00 00 00 80 00 10 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 41 					S: A5 5A 00 01 00 00 00 00 10 10		R: A5 5A 00 01 00 00 80 00 10 11 22 33 44 55 66 77 88 99 00 21 22 23 24 25 26 62 					S: A5 5A 00 02 00 00 00 00 10 11		R: A5 5A 00 02 00 00 80 00 10 21 22 23 24 25 26 27 28 29 20 21 22 23 24 25 26 D3 					S: A5 5A 00 03 00 00 00 00 10 12		R: A5 5A 00 03 00 00 80 00 10 31 32 33 34 35 36 37 38 39 30 31 32 33 34 35 36 D4					S: A5 5A 00 00 FF F5 00 00 10 03		R: A5 5A 00 00 FF F5 80 00 10 AA BB CC DD EE FF 11 22 33 44 55 66 77 88 99 00 7B }5. 控制器参数{		控制器参数从地址128开始记录，每个控制器占256字节长度的内容，即每个控制器开始的地址为128+256*n (n<=10)		#pragma pack(1)		typedef struct		{		    U16 totalTable[27];        					//总量曲线 单位10克		    U16 timeTStart[10];         				//饲喂时间开始		    U16 timeTEnd[10];           				//饲喂时间结束		    U16 timeTAmount[10];        				//饲喂总量		    U16 calTable[10];                   //校正表 -- g		    U16 canOver;                        //允许超出量                -- 10%		    U16 xlPer;                          //每次下料多少克 -- 100g		    U16 waterAuto;                      //是否自动下水		    U16 waterTime;                      //下水时间s		    U16 waterSpace;                     //下水间隔s		    U16 usePass;                        //是否使用密码		    U32 addr;                           //控制器地址		    U32 routeName;                      //路由器名称		    U32 routePass;                      //路由器密码		    U32 serverIP[4];                    //服务器IP地址		    U32 serverPort;                     //服务器端口		    U16 stage;                          //0 - 断奶 1 - 产前 2 - 哺乳		    U16 day;                            //第几日		    U16 hasEat;                         //今天已吃		    U16 eatDelay;                       //要求喂食延迟		    U16 sons;                           //仔猪头数		    U8  mday;                           //保存日		    U8  rev;		    U32 password;                       //用户密码		    U8  pN[2];                          //猪耳标前2位，ASCII码		    U8  rev1[2];                        //保留		    U32 pigNum;                         //猪耳标后8位		    U8  version[16];                    //版本信息		    U32 sum;                            //校验和		}__czqPara;		#pragma pack()}6. 控制器记录{		控制器参数从地址4096开始记录，每条记录16字节，每个控制器最多3200条记录， 		即每条记录开始的地址为4096+16*n (n<=3200)				typedef struct		{		    U32 pigNum;     //猪耳标		    U32 date;       //时标 从1970年1月1日0点0分0秒 到现在的秒数    										//时标为0或者0xffffffff时，记录无效		    U16 amount;     //总量		    U16 actual;     //实际		    U8  pN[2];      //耳标头2位，字母ASCII码		    U8  rev;        //保留		    U8  sum;        //校验		}__czqRec;		}