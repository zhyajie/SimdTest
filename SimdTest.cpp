
#include <iostream>
#include <arm_neon.h> //需要的头文件
#include <time.h>
#include <string.h>
#include <climits>
#include<sys/time.h>
using namespace std;
#define testTimes  1
#define uint8 unsigned char
#define uint32 unsigned int
#define uint16 unsigned short
#define int8 signed char
#define int16 signed short
//timeval结构定义为:
// struct timeval{
// long tv_sec; /*秒*/
// long tv_usec; /*微秒*/
// };
class TestNeon{
public: 
    TestNeon():dataLen(1000000),gemmM(100),gemmK(100),gemmN(100)
    {   
        arrFloat32=new float[dataLen];
        arrInt32=new int[dataLen];
        arrUint16=new uint16[dataLen];
        arrUint8=new uint8[dataLen];

        vecAddFloat32_A=new float[dataLen];
        vecAddFloat32_B=new float[dataLen];
        vecAddFloat32_C=new float[dataLen];
        vecAddInt32_A=new int[dataLen];
        vecAddInt32_B=new int[dataLen];
        vecAddInt32_C=new int[dataLen];
        vecAddUint16_A=new uint16[dataLen];
        vecAddUint16_B=new uint16[dataLen];
        vecAddUint16_C=new uint16[dataLen];
        vecAddUint8_A=new uint8[dataLen];
        vecAddUint8_B=new uint8[dataLen];
        vecAddUint8_C=new uint8[dataLen];

		vecMatrixFloat32_A=new float[4*4];
		vecMatrixFloat32_B=new float[4*4];
		vecMatrixFloat32_C=new float[4*4];
		
        arr2DGemmFloat32_A=new float*[gemmM];
        arr2DGemmFloat32_A[0]=new float [gemmM*gemmK];
        for(int i=1;i<gemmM;i++)
            arr2DGemmFloat32_A[i]=arr2DGemmFloat32_A[i-1]+gemmK;//二维数组的连续空间的定义
        arr2DGemmFloat32_B=new float*[gemmK];
        arr2DGemmFloat32_B[0]=new float [gemmK*gemmN];
        for(int i=1;i<gemmK;i++)
            arr2DGemmFloat32_B[i]=arr2DGemmFloat32_B[i-1]+gemmN;//二维数组的连续空间的定义
        arr2DGemmFloat32_C=new float*[gemmM];
        arr2DGemmFloat32_C[0]=new float [gemmM*gemmN];
        for(int i=1;i<gemmM;i++)
            arr2DGemmFloat32_C[i]=arr2DGemmFloat32_C[i-1]+gemmN;//二维数组的连续空间的定义
        // arr2DGemmInt32_A;
        // arr2DGemmInt32_B;
        // arr2DGemmInt32_C;
        // arr2DGemmUint16_A;
        // arr2DGemmUint16_B;
        // arr2DGemmUint16_C;
        
        //注意此处为C++代码，为方便孙单单理解开辟内存，注意在析构函数中delete掉
        new2DArr<uint8>(arr2DGemmUint8_A,gemmM,gemmK);
        new2DArr<uint8>(arr2DGemmUint8_B,gemmK,gemmN);
        new2DArr<uint8>(arr2DGemmUint8_C,gemmM,gemmN);

        // arr2DGemmUint8_A=new uint8 * [gemmM];
        // arr2DGemmUint8_A[0]=new uint8 [gemmM*gemmK];
        // for(int i=1;i<gemmM;i++)
        //     arr2DGemmUint8_A[i]=arr2DGemmUint8_A[i-1]+gemmK;//二维数组的连续空间的定义
        // arr2DGemmUint8_B=new uint8*[gemmK];
        // arr2DGemmUint8_B[0]=new uint8 [gemmK*gemmN];
        // for(int i=1;i<gemmK;i++)
        //     arr2DGemmUint8_B[i]=arr2DGemmUint8_B[i-1]+gemmN;//二维数组的连续空间的定义
        // arr2DGemmUint8_C=new uint8*[gemmM];
        // arr2DGemmUint8_C[0]=new uint8 [gemmM*gemmN];
        // for(int i=1;i<gemmM;i++)
        //     arr2DGemmUint8_C[i]=arr2DGemmUint8_C[i-1]+gemmN;//二维数组的连续空间的定义
		
		//定义uint16型数组，申请内存
		new2DArr<uint16>(arr2DGemmUint16_A,gemmM,gemmK);
        new2DArr<uint16>(arr2DGemmUint16_B,gemmK,gemmN);
        new2DArr<uint16>(arr2DGemmUint16_C,gemmM,gemmN);
		
		//定义int32型数组，申请内存
		// new2DArr<int32>(arr2DGemmInt32_A,gemmM,gemmK);
        // new2DArr<int32>(arr2DGemmInt32_B,gemmK,gemmN);
        // new2DArr<int32>(arr2DGemmInt32_C,gemmM,gemmN);
		arr2DGemmInt32_A=new int * [gemmM];
        arr2DGemmInt32_A[0]=new int [gemmM*gemmK];
        for(int i=1;i<gemmM;i++)
            arr2DGemmInt32_A[i]=arr2DGemmInt32_A[i-1]+gemmK;//二维数组的连续空间的定义
        arr2DGemmInt32_B=new int*[gemmK];
        arr2DGemmInt32_B[0]=new int [gemmK*gemmN];
        for(int i=1;i<gemmK;i++)
            arr2DGemmInt32_B[i]=arr2DGemmInt32_B[i-1]+gemmN;//二维数组的连续空间的定义
        arr2DGemmInt32_C=new int*[gemmM];
        arr2DGemmInt32_C[0]=new int [gemmM*gemmN];
        for(int i=1;i<gemmM;i++)
            arr2DGemmInt32_C[i]=arr2DGemmInt32_C[i-1]+gemmN;//二维数组的连续空间的定义


        copyFloat32_A=new float [dataLen];
        copyInt32_A=new int [dataLen];
		copyInt16_A=new int16 [dataLen];
		copyInt8_A=new int8 [dataLen];
		copyUint32_A=new uint32 [dataLen];
        copyUint16_A=new uint16 [dataLen];
        copyUint8_A=new uint8 [dataLen];
        copyFloat32_B=new float [dataLen];
        copyInt32_B=new int [dataLen];
		copyInt16_B=new int16 [dataLen];
		copyInt8_B=new int8 [dataLen];
		copyUint32_B=new uint32 [dataLen];
        copyUint16_B=new uint16 [dataLen];
        copyUint8_B=new uint8 [dataLen];

        init();
    }
    //注意此处为C++代码
    //参数说明： 1，二级指针的引用 2，矩阵行 3，矩阵列
    template <typename T>
    void new2DArr(T **&a,int rows,int cols){
        a=new T * [rows];
        a[0]=new T [rows*cols];
        for(int i=1;i<rows;i++)
            a[i]=a[i-1]+cols;//二维数组的连续空间的定义
    }
    void init(){
        for(int i=0;i<dataLen;i++){
           arrFloat32[i]=1.0;
            arrInt32[i]=1;
            arrUint8[i]=0;
            arrUint16[i]=0;

            vecAddFloat32_A[i]=8;
            vecAddFloat32_B[i]=2;
            vecAddInt32_A[i]=8;
            vecAddInt32_B[i]=2;
            vecAddUint16_A[i]=8;
            vecAddUint16_B[i]=2;
            vecAddUint8_A[i]=8;
            vecAddUint8_B[i]=2;
            copyFloat32_A[i]=i;
            if(i%2==0){
                copyInt32_A[i]=i%int(INT32_MAX);
                copyInt16_A[i]=i%int(INT16_MAX);
                copyInt8_A[i]=i%int(CHAR_MAX);
            }
            else{
                copyInt32_A[i]=-(i%int(INT32_MAX));
                copyInt16_A[i]=-(i%int(INT16_MAX));
                copyInt8_A[i]=-(i%int(CHAR_MAX));
            }
            
			copyUint32_A[i]=i%int(UINT32_MAX);
            copyUint16_A[i]=i%int(UINT16_MAX);
            copyUint8_A[i]=i%int(UCHAR_MAX);

        }
        for(int i=0;i<100;i++){//由于uint8数据表示范围为0~255，所以在求和时会数据溢出，所以将前100位数据置1、uint16同理
            arrUint8[i]=1;
            arrUint16[i]=1;
        }

        for(int i=0;i<gemmM;i++){
            for(int j=0;j<gemmK;j++){
                arr2DGemmFloat32_A[i][j]=8;
                arr2DGemmUint8_A[i][j]=1;
				arr2DGemmUint16_A[i][j]=1;
				arr2DGemmInt32_A[i][j]=2;
            }
        }
        for(int i=0;i<gemmK;i++){
            for(int j=0;j<gemmN;j++){
                arr2DGemmFloat32_B[i][j]=2;
                arr2DGemmUint8_B[i][j]=1;
				arr2DGemmUint16_B[i][j]=1;
				arr2DGemmInt32_B[i][j]=2;
            }
        }
        for(int i=0;i<gemmM;i++){
            for(int j=0;j<gemmN;j++){
                arr2DGemmFloat32_C[i][j]=0;
                arr2DGemmUint8_C[i][j]=0;
				arr2DGemmUint16_C[i][j]=0;
				arr2DGemmInt32_C[i][j]=0;
            }
        }
		for(int i=0;i<4;i++){
			for(int j=0;j<4;j++){
				vecMatrixFloat32_A[i*4+j]=i*4+j;
				vecMatrixFloat32_B[i*4+j]=i+j*5;
				vecMatrixFloat32_C[i*4+j]=0.0f;
			}
		}
    }
    void TestMain(){
        TestSumFloat32();
        TestSumInt32();
        TestSumUint16();
        TestSumUint8();
		
        TestVecAddFloat32();
        TestVecAddInt32();
        TestVecAddUint16();
        TestVecAddUint8();

        TestGemmFloat32();
        TestGemmInt32();
        TestGemmUint16();
        TestGemmUint8();

        TestCopyFloat32();
		TestCopyInt32();
		TestCopyUint16();
        TestCopyUint8();
		
		TestMatrixMulFloat32();
		
		TestCopyFloat32ToUint8();
		
		TestConvertInt8ToUint16();

        TestConvertInt8ToUint8();

        TestConvertUint8ToInt8();
		
		TestVcvtf32();

    }
    void printTestResult(string info,double dur)
	{
        if(dur<=1000)
		    cout << info << "：" << dur <<"us"<<endl;
        if(dur>1000&&dur<=1000000)
		    cout << info << "：" << dur/1000 <<"ms"<<endl;
        if(dur>1000000)
		    cout << info << ": " << dur/1000000 <<"s"<<endl;
	}
    void TestSumFloat32(){
		
        float result,resutltNeon;
        struct timeval t1,t2;
        double timeuse;
        gettimeofday(&t1,NULL);
        for(int i=0;i<testTimes;i++){
                result=SumFloat32(arrFloat32,dataLen);
        }
        gettimeofday(&t2,NULL);
        timeuse = (t2.tv_sec - t1.tv_sec)*1000000.0 + (t2.tv_usec - t1.tv_usec);
        printTestResult("SumFloat32",timeuse);
        gettimeofday(&t1,NULL);
        for(int i=0;i<testTimes;i++){
            resutltNeon=SumFloat32Neon(arrFloat32,dataLen);

        }
        gettimeofday(&t2,NULL);
        timeuse = (t2.tv_sec - t1.tv_sec)*1000000.0 + (t2.tv_usec - t1.tv_usec);
        
        printTestResult("SumFloat32Neon",timeuse);
        
        if(result!=resutltNeon)
            printTestResult("TestSumFloat32 test error",-1.0);
        cout<<endl;
    }
    void TestSumInt32(){
		float result,resutltNeon;
        struct timeval t1,t2;
        double timeuse;
        gettimeofday(&t1,NULL);
        for(int i=0;i<testTimes;i++){
                result=SumInt32(arrInt32,dataLen);
        }
        gettimeofday(&t2,NULL);
        timeuse = (t2.tv_sec - t1.tv_sec)*1000000.0 + (t2.tv_usec - t1.tv_usec);
        printTestResult("SumInt32",timeuse);
        gettimeofday(&t1,NULL);
        for(int i=0;i<testTimes;i++){
            resutltNeon=SumInt32Neon(arrInt32,dataLen);

        }
        gettimeofday(&t2,NULL);
        timeuse = (t2.tv_sec - t1.tv_sec)*1000000.0 + (t2.tv_usec - t1.tv_usec);
        
        printTestResult("SumInt32Neon",timeuse);
        if(result!=resutltNeon)
            printTestResult("TestSumInt32 test error",-1.0);
        cout<<endl;
    }
    void TestSumUint8(){
		float result,resutltNeon;
        struct timeval t1,t2;
        double timeuse;
        gettimeofday(&t1,NULL);
        for(int i=0;i<testTimes;i++){
                result=SumUint8(arrUint8,dataLen);
        }
        gettimeofday(&t2,NULL);
        timeuse = (t2.tv_sec - t1.tv_sec)*1000000.0 + (t2.tv_usec - t1.tv_usec);
        printTestResult("SumUint8",timeuse);
        gettimeofday(&t1,NULL);
        for(int i=0;i<testTimes;i++){
            resutltNeon=SumUint8Neon(arrUint8,dataLen);

        }
        gettimeofday(&t2,NULL);
        timeuse = (t2.tv_sec - t1.tv_sec)*1000000.0 + (t2.tv_usec - t1.tv_usec);
        
        printTestResult("SumUint8Neon",timeuse);
        if(result!=resutltNeon)
            printTestResult("TestSumUint8 test error",-1.0);
        cout<<endl;
    }
	
	void TestSumUint16(){
		float result,resutltNeon;
        struct timeval t1,t2;
        double timeuse;
        gettimeofday(&t1,NULL);
        for(int i=0;i<testTimes;i++){
                result=SumUint16(arrUint16,dataLen);
        }
        gettimeofday(&t2,NULL);
        timeuse = (t2.tv_sec - t1.tv_sec)*1000000.0 + (t2.tv_usec - t1.tv_usec);
        printTestResult("SumUint16",timeuse);
        gettimeofday(&t1,NULL);
        for(int i=0;i<testTimes;i++){
            resutltNeon=SumUint16Neon(arrUint16,dataLen);

        }
        gettimeofday(&t2,NULL);
        timeuse = (t2.tv_sec - t1.tv_sec)*1000000.0 + (t2.tv_usec - t1.tv_usec);
        
        printTestResult("SumUint16Neon",timeuse);
        if(result!=resutltNeon)
            printTestResult("TestSumUint16 test error",-1.0);
        cout<<endl;
    }
    void TestVecAddFloat32(){
        struct timeval t1,t2;
        double timeuse;
        gettimeofday(&t1,NULL);
        for(int i=0;i<testTimes;i++){
            VecAddFloat32(vecAddFloat32_A,vecAddFloat32_B,vecAddFloat32_C,dataLen);
        }
        gettimeofday(&t2,NULL);
        timeuse = (t2.tv_sec - t1.tv_sec)*1000000.0 + (t2.tv_usec - t1.tv_usec);
        printTestResult("VecAddFloat32",timeuse);
        gettimeofday(&t1,NULL);
        for(int i=0;i<testTimes;i++){
            VecAddFloat32Neon(vecAddFloat32_A,vecAddFloat32_B,vecAddFloat32_C,dataLen);
        }
        gettimeofday(&t2,NULL);
        timeuse = (t2.tv_sec - t1.tv_sec)*1000000.0 + (t2.tv_usec - t1.tv_usec);
        printTestResult("VecAddFloat32Neon",timeuse);
        cout<<endl;

    }
	void TestVecAddInt32(){
        struct timeval t1,t2;
        double timeuse;
        gettimeofday(&t1,NULL);
        for(int i=0;i<testTimes;i++){
            VecAddInt32(vecAddInt32_A,vecAddInt32_B,vecAddInt32_C,dataLen);
        }
        gettimeofday(&t2,NULL);
        timeuse = (t2.tv_sec - t1.tv_sec)*1000000.0 + (t2.tv_usec - t1.tv_usec);
        printTestResult("VecAddInt32",timeuse);
        gettimeofday(&t1,NULL);
        for(int i=0;i<testTimes;i++){
            VecAddInt32Neon(vecAddInt32_A,vecAddInt32_B,vecAddInt32_C,dataLen);
        }
        gettimeofday(&t2,NULL);
        timeuse = (t2.tv_sec - t1.tv_sec)*1000000.0 + (t2.tv_usec - t1.tv_usec);
        printTestResult("VecAddInt32Neon",timeuse);
        cout<<endl;
    }
	void TestVecAddUint8(){
		struct timeval t1,t2;
        double timeuse;
        gettimeofday(&t1,NULL);
        for(int i=0;i<testTimes;i++){
            VecAddUint8(vecAddUint8_A,vecAddUint8_B,vecAddUint8_C,dataLen);
        }
        gettimeofday(&t2,NULL);
        timeuse = (t2.tv_sec - t1.tv_sec)*1000000.0 + (t2.tv_usec - t1.tv_usec);
        printTestResult("VecAddUint8",timeuse);
        gettimeofday(&t1,NULL);
        for(int i=0;i<testTimes;i++){
            VecAddUint8Neon(vecAddUint8_A,vecAddUint8_B,vecAddUint8_C,dataLen);
        }
        gettimeofday(&t2,NULL);
        timeuse = (t2.tv_sec - t1.tv_sec)*1000000.0 + (t2.tv_usec - t1.tv_usec);
        printTestResult("VecAddUint8Neon",timeuse);
        cout<<endl;
	}
	void TestVecAddUint16(){
		struct timeval t1,t2;
        double timeuse;
        gettimeofday(&t1,NULL);
        for(int i=0;i<testTimes;i++){
            VecAddUint16(vecAddUint16_A,vecAddUint16_B,vecAddUint16_C,dataLen);
        }
        gettimeofday(&t2,NULL);
        timeuse = (t2.tv_sec - t1.tv_sec)*1000000.0 + (t2.tv_usec - t1.tv_usec);
        printTestResult("VecAddUint16",timeuse);
        gettimeofday(&t1,NULL);
        for(int i=0;i<testTimes;i++){
            VecAddUint16Neon(vecAddUint16_A,vecAddUint16_B,vecAddUint16_C,dataLen);
        }
        gettimeofday(&t2,NULL);
        timeuse = (t2.tv_sec - t1.tv_sec)*1000000.0 + (t2.tv_usec - t1.tv_usec);
        printTestResult("VecAddUint16Neon",timeuse);
        cout<<endl;
	}
	

     void TestGemmFloat32(){
        struct timeval t1,t2;
        double timeuse;
        gettimeofday(&t1,NULL);
        for(int i=0;i<testTimes;i++){
            GemmFloat32(gemmM,gemmK,gemmN,arr2DGemmFloat32_A,arr2DGemmFloat32_B,arr2DGemmFloat32_C);
        }
        gettimeofday(&t2,NULL);
        timeuse = (t2.tv_sec - t1.tv_sec)*1000000.0 + (t2.tv_usec - t1.tv_usec);
        printTestResult("GemmFloat32",timeuse);
        //重新开辟一段内存将计算结果保存为了验证结果正确性
         //也可以用定义的C++模板函数   new2DArr<float>(res,gemmM,gemmN); 
        float **res=new float*[gemmM];
        res[0]=new float[gemmM*gemmN];
        for(int i=1;i<gemmM;i++)
            res[i]=res[i-1]+gemmN;
        for(int i=0;i<gemmM;i++){
            for(int j=0;j<gemmN;j++){
                res[i][j]=arr2DGemmFloat32_C[i][j];
            }
        }
       
        gettimeofday(&t1,NULL);
        for(int i=0;i<testTimes;i++){
            GemmFloat32Neon(gemmM,gemmK,gemmN,arr2DGemmFloat32_A,arr2DGemmFloat32_B,arr2DGemmFloat32_C);
        }
        gettimeofday(&t2,NULL);
        timeuse = (t2.tv_sec - t1.tv_sec)*1000000.0 + (t2.tv_usec - t1.tv_usec);
        printTestResult("GemmFloat32Neon",timeuse);
        //验证结果的正确性
        for(int i=0;i<gemmM;i++){
            for(int j=0;j<gemmN;j++){
                if(res[i][j]!=arr2DGemmFloat32_C[i][j])
                    cout<<"GemmFloat32 error"<<":i="<<i<<" j="<<j<<endl;
            }
        }
        delete [] res;//释放开辟的内存块
        cout<<endl;
    }
    void TestGemmUint8(){
        struct timeval t1,t2;
        double timeuse;
        gettimeofday(&t1,NULL);
        for(int i=0;i<testTimes;i++){
            GemmUint8(gemmM,gemmK,gemmN,arr2DGemmUint8_A,arr2DGemmUint8_B,arr2DGemmUint8_C);
        }
        gettimeofday(&t2,NULL);
        timeuse = (t2.tv_sec - t1.tv_sec)*1000000.0 + (t2.tv_usec - t1.tv_usec);
        printTestResult("GemmUint8",timeuse);
        //重新开辟一段内存将计算结果保存为了验证结果正确性
         //也可以用定义的C++模板函数   new2DArr<uint8>(res,gemmM,gemmN); 
        uint8 **res=new uint8*[gemmM];
        res[0]=new uint8[gemmM*gemmN];
        for(int i=1;i<gemmM;i++)
            res[i]=res[i-1]+gemmN;
        for(int i=0;i<gemmM;i++){
            for(int j=0;j<gemmN;j++){
                res[i][j]=arr2DGemmUint8_C[i][j];
            }
        }
       
        gettimeofday(&t1,NULL);
        for(int i=0;i<testTimes;i++){
            GemmUint8Neon(gemmM,gemmK,gemmN,arr2DGemmUint8_A,arr2DGemmUint8_B,arr2DGemmUint8_C);
        }
        gettimeofday(&t2,NULL);
        timeuse = (t2.tv_sec - t1.tv_sec)*1000000.0 + (t2.tv_usec - t1.tv_usec);
        printTestResult("GemmUint8Neon",timeuse);
        //验证结果的正确性
        for(int i=0;i<gemmM;i++){
            for(int j=0;j<gemmN;j++){
                if(res[i][j]!=arr2DGemmUint8_C[i][j])
                    cout<<"GemmUint8 error"<<":i="<<i<<" j="<<j<<endl;
            }
        }
        delete [] res;//释放开辟的内存块
        cout<<endl;
    }
	void TestGemmUint16(){
        struct timeval t1,t2;
        double timeuse;
        gettimeofday(&t1,NULL);
        for(int i=0;i<testTimes;i++){
            GemmUint16(gemmM,gemmK,gemmN,arr2DGemmUint16_A,arr2DGemmUint16_B,arr2DGemmUint16_C);
        }
        gettimeofday(&t2,NULL);
        timeuse = (t2.tv_sec - t1.tv_sec)*1000000.0 + (t2.tv_usec - t1.tv_usec);
        printTestResult("GemmUint16",timeuse);
        //重新开辟一段内存将计算结果保存为了验证结果正确性
         //也可以用定义的C++模板函数   new2DArr<uint16>(res,gemmM,gemmN); 下面使用C++模板函数定义
        uint16 **res=new uint16*[gemmM];
        res[0]=new uint16[gemmM*gemmN];
        for(int i=1;i<gemmM;i++)
            res[i]=res[i-1]+gemmN;
        for(int i=0;i<gemmM;i++){
            for(int j=0;j<gemmN;j++){
                res[i][j]=arr2DGemmUint16_C[i][j];
            }
        }
       
        gettimeofday(&t1,NULL);
        for(int i=0;i<testTimes;i++){
            GemmUint16Neon(gemmM,gemmK,gemmN,arr2DGemmUint16_A,arr2DGemmUint16_B,arr2DGemmUint16_C);
        }
        gettimeofday(&t2,NULL);
        timeuse = (t2.tv_sec - t1.tv_sec)*1000000.0 + (t2.tv_usec - t1.tv_usec);
        printTestResult("GemmUint16Neon",timeuse);
        //验证结果的正确性
        for(int i=0;i<gemmM;i++){
            for(int j=0;j<gemmN;j++){
                if(res[i][j]!=arr2DGemmUint16_C[i][j])
                    cout<<"GemmUint16 error"<<":i="<<i<<" j="<<j<<endl;
            }
        }
        delete [] res;//释放开辟的内存块
        cout<<endl;
    }
	void TestGemmInt32(){
        struct timeval t1,t2;
        double timeuse;
        gettimeofday(&t1,NULL);
        for(int i=0;i<testTimes;i++){
            GemmInt32(gemmM,gemmK,gemmN,arr2DGemmInt32_A,arr2DGemmInt32_B,arr2DGemmInt32_C);
        }
        gettimeofday(&t2,NULL);
        timeuse = (t2.tv_sec - t1.tv_sec)*1000000.0 + (t2.tv_usec - t1.tv_usec);
        printTestResult("GemmInt32",timeuse);
        //重新开辟一段内存将计算结果保存为了验证结果正确性
         //也可以用定义的C++模板函数   new2DArr<int32>(res,gemmM,gemmN); 下面使用C++模板函数定义
        int **res=new int*[gemmM];
        res[0]=new int[gemmM*gemmN];
        for(int i=1;i<gemmM;i++)
            res[i]=res[i-1]+gemmN;
        for(int i=0;i<gemmM;i++){
            for(int j=0;j<gemmN;j++){
                res[i][j]=arr2DGemmInt32_C[i][j];
            }
        }
       
        gettimeofday(&t1,NULL);
        for(int i=0;i<testTimes;i++){
            GemmInt32Neon(gemmM,gemmK,gemmN,arr2DGemmInt32_A,arr2DGemmInt32_B,arr2DGemmInt32_C);
        }
        gettimeofday(&t2,NULL);
        timeuse = (t2.tv_sec - t1.tv_sec)*1000000.0 + (t2.tv_usec - t1.tv_usec);
        printTestResult("GemmInt32Neon",timeuse);
        //验证结果的正确性
        for(int i=0;i<gemmM;i++){
            for(int j=0;j<gemmN;j++){
                if(res[i][j]!=arr2DGemmInt32_C[i][j])
                    cout<<"GemmInt32 error"<<":i="<<i<<" j="<<j<<endl;
            }
        }
        delete [] res;//释放开辟的内存块
        cout<<endl;
    }
    void TestCopyFloat32(){
        struct timeval t1,t2;
        double timeuse;
        gettimeofday(&t1,NULL);
        for(int i=0;i<testTimes;i++){
            CopyFloat32(copyFloat32_A,copyFloat32_B,dataLen);
        }
        gettimeofday(&t2,NULL);
        timeuse = (t2.tv_sec - t1.tv_sec)*1000000.0 + (t2.tv_usec - t1.tv_usec);
        printTestResult("CopyFloat32",timeuse);
        //重新开辟一段内存将计算结果保存为了验证结果正确性
        float *res=new float [dataLen];
       for(int i=0;i<dataLen;i++)
            res[i]=copyFloat32_B[i];
        gettimeofday(&t1,NULL);
        for(int i=0;i<testTimes;i++){
            CopyFloat32Neon(copyFloat32_A,copyFloat32_B,dataLen);
        }
        gettimeofday(&t2,NULL);
        timeuse = (t2.tv_sec - t1.tv_sec)*1000000.0 + (t2.tv_usec - t1.tv_usec);
        printTestResult("CopyFloat32Neon",timeuse);
        //验证结果的正确性
        for(int i=0;i<gemmM;i++){
                if(res[i]!=copyFloat32_B[i])
                    cout<<"CopyFloat32 error"<<":i="<<i<<endl;
        }
        delete [] res;//释放开辟的内存块
        cout<<endl;
    }
	
	void TestCopyInt32(){
        struct timeval t1,t2;
        double timeuse;
        gettimeofday(&t1,NULL);
        for(int i=0;i<testTimes;i++){
            CopyInt32(copyInt32_A,copyInt32_B,dataLen);
        }
        gettimeofday(&t2,NULL);
        timeuse = (t2.tv_sec - t1.tv_sec)*1000000.0 + (t2.tv_usec - t1.tv_usec);
        printTestResult("CopyInt32",timeuse);
        //重新开辟一段内存将计算结果保存为了验证结果正确性
        int *res=new int [dataLen];
       for(int i=0;i<dataLen;i++)
            res[i]=copyInt32_B[i];
        gettimeofday(&t1,NULL);
        for(int i=0;i<testTimes;i++){
            CopyInt32Neon(copyInt32_A,copyInt32_B,dataLen);
        }
        gettimeofday(&t2,NULL);
        timeuse = (t2.tv_sec - t1.tv_sec)*1000000.0 + (t2.tv_usec - t1.tv_usec);
        printTestResult("CopyInt32Neon",timeuse);
        //验证结果的正确性
        for(int i=0;i<gemmM;i++){
                if(res[i]!=copyInt32_B[i])
                    cout<<"CopyInt32 error"<<":i="<<i<<endl;
        }
        delete [] res;//释放开辟的内存块
        cout<<endl;
    }
	
	void TestCopyUint16(){
        struct timeval t1,t2;
        double timeuse;
        gettimeofday(&t1,NULL);
        for(int i=0;i<testTimes;i++){
            CopyUint16(copyUint16_A,copyUint16_B,dataLen);
        }
        gettimeofday(&t2,NULL);
        timeuse = (t2.tv_sec - t1.tv_sec)*1000000.0 + (t2.tv_usec - t1.tv_usec);
        printTestResult("CopyUint16",timeuse);
        //重新开辟一段内存将计算结果保存为了验证结果正确性
        uint16 *res=new uint16 [dataLen];
       for(int i=0;i<dataLen;i++)
            res[i]=copyUint16_B[i];
        gettimeofday(&t1,NULL);
        for(int i=0;i<testTimes;i++){
            CopyUint16Neon(copyUint16_A,copyUint16_B,dataLen);
        }
        gettimeofday(&t2,NULL);
        timeuse = (t2.tv_sec - t1.tv_sec)*1000000.0 + (t2.tv_usec - t1.tv_usec);
        printTestResult("CopyUint16Neon",timeuse);
        //验证结果的正确性
        for(int i=0;i<gemmM;i++){
                if(res[i]!=copyUint16_B[i])
                    cout<<"CopyUint16 error"<<":i="<<i<<endl;
        }
        delete [] res;//释放开辟的内存块
        cout<<endl;
    }

    void TestCopyUint8(){
        struct timeval t1,t2;
        double timeuse;
        gettimeofday(&t1,NULL);
        for(int i=0;i<testTimes;i++){
            CopyUint8(copyUint8_A,copyUint8_B,dataLen);
        }
        gettimeofday(&t2,NULL);
        timeuse = (t2.tv_sec - t1.tv_sec)*1000000.0 + (t2.tv_usec - t1.tv_usec);
        printTestResult("CopyUint8",timeuse);
        //重新开辟一段内存将计算结果保存为了验证结果正确性
        uint8 *res=new uint8 [dataLen];
       for(int i=0;i<dataLen;i++){
           res[i]=copyUint8_B[i];
           copyUint8_B[i]=0;
       }
            
        gettimeofday(&t1,NULL);
        for(int i=0;i<testTimes;i++){
            CopyUint8Neon(copyUint8_A,copyUint8_B,dataLen);
        }
        gettimeofday(&t2,NULL);
        timeuse = (t2.tv_sec - t1.tv_sec)*1000000.0 + (t2.tv_usec - t1.tv_usec);
        printTestResult("CopyUint8Neon",timeuse);
        //验证结果的正确性
        for(int i=0;i<gemmM;i++){
                if(res[i]!=copyUint8_B[i])
                    cout<<"CopyUint8 error"<<":i="<<i<<endl;
        }
        delete [] res;//释放开辟的内存块
        cout<<endl;
    }
	
	void TestMatrixMulFloat32(){
        struct timeval t1,t2;
        double timeuse;
        gettimeofday(&t1,NULL);
        for(int i=0;i<testTimes;i++){
            MatrixMulFloat32(vecMatrixFloat32_A,vecMatrixFloat32_B,vecMatrixFloat32_C);
        }
        gettimeofday(&t2,NULL);
        timeuse = (t2.tv_sec - t1.tv_sec)*1000000.0 + (t2.tv_usec - t1.tv_usec);
        printTestResult("MatrixMulFloat32",timeuse);
        //重新开辟一段内存将计算结果保存为了验证结果正确性
         //也可以用定义的C++模板函数   new2DArr<float>(res,gemmM,gemmN); 
        float *res=new float[4*4];
        for(int i=0;i<4*4;i++){
            res[i]=vecMatrixFloat32_C[i];
        }
        gettimeofday(&t1,NULL);
        for(int i=0;i<testTimes;i++){
            MatrixMulFloat32Neon(vecMatrixFloat32_A,vecMatrixFloat32_B,vecMatrixFloat32_C);
        }
        gettimeofday(&t2,NULL);
        timeuse = (t2.tv_sec - t1.tv_sec)*1000000.0 + (t2.tv_usec - t1.tv_usec);
        printTestResult("MatrixMulFloat32Neon",timeuse);
        //验证结果的正确性
        for(int i=0;i<4*4;i++){
            if(res[i]!=vecMatrixFloat32_C[i])
                    cout<<"MatrixMulFloat32 error"<<":i="<<i<<endl;
        }
        delete [] res;//释放开辟的内存块
        cout<<endl;
    }
	
	void TestCopyFloat32ToUint8(){
		struct timeval t1,t2;
        double timeuse;
        gettimeofday(&t1,NULL);
        for(int i=0;i<testTimes;i++){
            CopyFloat32ToUint8(copyFloat32_A,copyUint8_B,dataLen);
        }
        gettimeofday(&t2,NULL);
        timeuse = (t2.tv_sec - t1.tv_sec)*1000000.0 + (t2.tv_usec - t1.tv_usec);
        printTestResult("CopyFloat32ToUint8",timeuse);
        //重新开辟一段内存将计算结果保存为了验证结果正确性
        uint8 *res=new uint8 [dataLen];
       for(int i=0;i<dataLen;i++){
           res[i]=copyUint8_B[i];
           copyUint8_B[i]=0;
       }
            
        gettimeofday(&t1,NULL);
        for(int i=0;i<testTimes;i++){
            CopyFloat32ToUint8Neon(copyFloat32_A,copyUint8_B,dataLen);
        }
        gettimeofday(&t2,NULL);
        timeuse = (t2.tv_sec - t1.tv_sec)*1000000.0 + (t2.tv_usec - t1.tv_usec);
        printTestResult("CopyFloat32ToUint8Neon",timeuse);
        //验证结果的正确性
        for(int i=0;i<gemmM;i++){
                if(res[i]!=copyUint8_B[i])
                    cout<<"CopyFloat32ToUint8 error"<<":i="<<i<<endl;
        }
        delete [] res;//释放开辟的内存块
        cout<<endl;
	}
	
	void TestConvertInt8ToUint16(){
		struct timeval t1,t2;
        double timeuse;
        gettimeofday(&t1,NULL);
        for(int i=0;i<testTimes;i++){
            ConvertInt8ToUint16(copyInt8_A,copyUint16_B,dataLen);
        }
        gettimeofday(&t2,NULL);
        timeuse = (t2.tv_sec - t1.tv_sec)*1000000.0 + (t2.tv_usec - t1.tv_usec);
        printTestResult("ConvertInt8ToUint16",timeuse);
        //重新开辟一段内存将计算结果保存为了验证结果正确性
        uint16 *res=new uint16[dataLen];
       for(int i=0;i<dataLen;i++){
           res[i]=copyUint16_B[i];
           //copyUint16_B[i]=0;
       }
            
        gettimeofday(&t1,NULL);
        for(int i=0;i<testTimes;i++){
            ConvertInt8ToUint16Neon(copyInt8_A,copyUint16_B,dataLen);
        }
        gettimeofday(&t2,NULL);
        timeuse = (t2.tv_sec - t1.tv_sec)*1000000.0 + (t2.tv_usec - t1.tv_usec);
        printTestResult("ConvertInt8ToUint16Neon",timeuse);
        //验证结果的正确性
        for(int i=0;i<gemmM;i++){
                if(res[i]!=copyUint16_B[i])
                    cout<<"ConvertInt8ToUint16 error"<<":i="<<i<<endl;
        }
        delete [] res;//释放开辟的内存块
        cout<<endl;
	}
	
    void TestConvertInt8ToUint8(){
		struct timeval t1,t2;
        double timeuse;
        gettimeofday(&t1,NULL);
        for(int i=0;i<testTimes;i++){
            ConvertInt8ToUint8(copyInt8_A,copyUint8_B,dataLen);
        }
        gettimeofday(&t2,NULL);
        timeuse = (t2.tv_sec - t1.tv_sec)*1000000.0 + (t2.tv_usec - t1.tv_usec);
        printTestResult("ConvertInt8ToUint8",timeuse);
        //重新开辟一段内存将计算结果保存为了验证结果正确性
        uint8 *res=new uint8[dataLen];
       for(int i=0;i<dataLen;i++){
           res[i]=copyUint8_B[i];
           //copyUint16_B[i]=0;
       }
            
        gettimeofday(&t1,NULL);
        for(int i=0;i<testTimes;i++){
            ConvertInt8ToUint8Neon(copyInt8_A,copyUint8_B,dataLen);
        }
        gettimeofday(&t2,NULL);
        timeuse = (t2.tv_sec - t1.tv_sec)*1000000.0 + (t2.tv_usec - t1.tv_usec);
        printTestResult("ConvertInt8ToUint8Neon",timeuse);
        //验证结果的正确性
        for(int i=0;i<gemmM;i++){
                if(res[i]!=copyUint8_B[i])
                    cout<<"ConvertInt8ToUint8 error"<<":i="<<i<<endl;
        }
        delete [] res;//释放开辟的内存块
        cout<<endl;
	}
    void TestConvertUint8ToInt8(){
		struct timeval t1,t2;
        double timeuse;
        gettimeofday(&t1,NULL);
        for(int i=0;i<testTimes;i++){
            ConvertUint8ToInt8(copyUint8_A,copyInt8_B,dataLen);
        }
        gettimeofday(&t2,NULL);
        timeuse = (t2.tv_sec - t1.tv_sec)*1000000.0 + (t2.tv_usec - t1.tv_usec);
        printTestResult("ConvertUint8ToInt8",timeuse);
        //重新开辟一段内存将计算结果保存为了验证结果正确性
        uint8 *res=new uint8[dataLen];
       for(int i=0;i<dataLen;i++){
           res[i]=copyUint8_B[i];
           //copyUint16_B[i]=0;
       }
            
        gettimeofday(&t1,NULL);
        for(int i=0;i<testTimes;i++){
            ConvertUint8ToInt8Neon(copyUint8_A,copyInt8_B,dataLen);
        }
        gettimeofday(&t2,NULL);
        timeuse = (t2.tv_sec - t1.tv_sec)*1000000.0 + (t2.tv_usec - t1.tv_usec);
        printTestResult("ConvertUint8ToInt8Neon",timeuse);
        //验证结果的正确性
        for(int i=0;i<gemmM;i++){
                if(res[i]!=copyUint8_B[i])
                    cout<<"ConvertUint8ToInt8Neon error"<<":i="<<i<<endl;
        }
        delete [] res;//释放开辟的内存块
        cout<<endl;
	}
    float SumFloat32(float *array, int len)
    {
        float *arr=array;
        float sum(0.0);
        for(int j=0;j<len;j++)
            sum += *arr++;
        return sum;
    }

    float SumFloat32Neon(float *array, int len)
    {
        float *arr=array;
        int dim4; // 数组长度除4整数
        int left4; // 数组长度除4余数
        float32x4_t sum_vec;//定义用于暂存累加结果的寄存器且初始化为0
        float sum=0.0;
        dim4 = len >> 2; // 数组长度除4整数
        left4 = len & 3; // 数组长度除4余数
        sum_vec = vdupq_n_f32(0.0);
        for (; dim4>0; dim4--, arr+=4) //每次同时访问4个数组元素
        {
            float32x4_t data_vec = vld1q_f32(arr); //依次取4个元素存入寄存器vec
            sum_vec = vaddq_f32(sum_vec, data_vec);//ri = ai + bi 计算两组寄存器对应元素之和并存放到相应结果
        }
        sum = vgetq_lane_f32(sum_vec, 0)+vgetq_lane_f32(sum_vec, 1)+vgetq_lane_f32(sum_vec, 2)+vgetq_lane_f32(sum_vec, 3);//将累加结果寄存器中的所有元素相加得到最终累加值
        for (; left4>0; left4--, arr++)
            sum += (*arr) ;   //对于剩下的少于4的数字，依次计算累加即可
        return sum;   
    }
   

    int SumInt32(int *array, int len)
    {
        int *arr=array;
        int sum(0);
        
        sum=0.0;
            for(int j=0;j<len;j++)
                sum += *arr++;
        return sum;
    }

    int SumInt32Neon(int  *array, int len)
    {
        int *arr=array;   
        int dim4 ; // 数组长度除4整数
        int left4; // 数组长度除4余数
        int32x4_t sum_vec = vdupq_n_s32(0);//定义用于暂存累加结果的寄存器且初始化为0
        int sum=0.0;
        dim4 = len >> 2; // 数组长度除4整数
        left4 = len & 3; // 数组长度除4余数
        for (; dim4>0; dim4--, arr+=4) //每次同时访问4个数组元素
        {
            int32x4_t data_vec = vld1q_s32(arr); //依次取4个元素存入寄存器vec
            sum_vec = vaddq_s32(sum_vec, data_vec);//ri = ai + bi 计算两组寄存器对应元素之和并存放到相应结果
        }
        sum = vgetq_lane_s32(sum_vec, 0)+vgetq_lane_s32(sum_vec, 1)+vgetq_lane_s32(sum_vec, 2)+vgetq_lane_s32(sum_vec, 3);//���ۼӽ���Ĵ����е�����Ԫ����ӵõ������ۼ�ֵ
        for (; left4>0; left4--, arr++)
            sum += (*arr) ;   //对于剩下的少于4的数字，依次计算累加即可
        return sum;
    }

    int SumUint8( uint8 *array, int len)
    {
        uint8 *arr=array;
        int sum(0);
        for(int j=0;j<len;j++)
            sum += *arr++;
        return sum;
    }

    int SumUint8Neon(uint8 *array, int len)
    {   
        uint8 *arr=array;
        int dim16 ; // 数组长度除16整数
        int left16; // 数组长度除16余数
        uint8x16_t sum_vec = vdupq_n_u8(0);//定义用于暂存累加结果的寄存器且初始化为0
        int sum=0;
        
        dim16 = len >> 4; // 数组长度除16整数
        left16 = len % 16; // 数组长度除16余数
        
        for (; dim16>0; dim16--, arr+=16) //每次同时访问16个数组元素
        {
            uint8x16_t data_vec = vld1q_u8(arr); //依次取16个元素存入寄存器vec
            sum_vec = vaddq_u8(sum_vec, data_vec);//ri = ai + bi 计算两组寄存器对应元素之和并存放到相应结果
        }
        //将16通道的值依次加起来
        sum=vgetq_lane_u8(sum_vec, 0)+vgetq_lane_u8(sum_vec,1)
        +vgetq_lane_u8(sum_vec, 2)+vgetq_lane_u8(sum_vec, 3)
        +vgetq_lane_u8(sum_vec, 4)+vgetq_lane_u8(sum_vec, 5)
        +vgetq_lane_u8(sum_vec, 6)+vgetq_lane_u8(sum_vec, 7)
        +vgetq_lane_u8(sum_vec, 8)+vgetq_lane_u8(sum_vec, 9)
        +vgetq_lane_u8(sum_vec, 10)+vgetq_lane_u8(sum_vec, 11)
        +vgetq_lane_u8(sum_vec, 12)+vgetq_lane_u8(sum_vec, 13)
        +vgetq_lane_u8(sum_vec, 14)+vgetq_lane_u8(sum_vec, 15);//注意此处第二个参数为const int 类型  不能采用for 循环

        for (; left16>0; left16--, arr++)
            sum += (*arr) ;   //对于剩下的少于16的数字，依次计算累加即可
        return sum;
    }
	
	int SumUint16( uint16 *array, int len)
    {
        uint16 *arr=array;
        int sum(0);
        for(int j=0;j<len;j++)
            sum += *arr++;
        return sum;
    }

    int SumUint16Neon(uint16 *array, int len)
    {   
        uint16 *arr=array;
        int dim8 ; // 数组长度除8整数
        int left8; // 数组长度除8余数
        uint16x8_t sum_vec = vdupq_n_u16(0);//定义用于暂存累加结果的寄存器且初始化为0
        int sum=0;
        
        dim8 = len >> 3; // 数组长度除8整数
        left8 = len % 8; // 数组长度除8余数
        
        for (; dim8>0; dim8--, arr+=8) //每次同时访问8个数组元素
        {
            uint16x8_t data_vec = vld1q_u16(arr); //依次取8个元素存入寄存器vec
            sum_vec = vaddq_u16(sum_vec, data_vec);//ri = ai + bi 计算两组寄存器对应元素之和并存放到相应结果
        }
        //将8通道的值依次加起来
        sum=vgetq_lane_u16(sum_vec, 0)+vgetq_lane_u16(sum_vec,1)
        +vgetq_lane_u16(sum_vec, 2)+vgetq_lane_u16(sum_vec, 3)
        +vgetq_lane_u16(sum_vec, 4)+vgetq_lane_u16(sum_vec, 5)
        +vgetq_lane_u16(sum_vec, 6)+vgetq_lane_u16(sum_vec, 7);//注意此处第二个参数为const int 类型  不能采用for 循环

        for (; left8>0; left8--, arr++)
            sum += (*arr) ;   //对于剩下的少于8的数字，依次计算累加即可
        return sum;
    }
	
    void VecAddFloat32(float *vecA ,float *vecB,float *vecC,int len){
        float *A=vecA;
        float *B=vecB;
        float *C=vecC;
        for(int i=0;i<len;i++){
            C[i]=A[i]+B[i];//加减乘
            C[i]+=A[i]-B[i];
            C[i]+=A[i]*B[i];
        }
        
    }
	
    void VecAddFloat32Neon(float *vecA ,float *vecB,float *vecC,int len){
        float *A=vecA;
        float *B=vecB;
        float *C=vecC;
        int dim4= len >> 2; // 数组长度除4整数
        int left4= len & 3; // 数组长度除4余数
        for (; dim4>0; dim4--, A+=4,B+=4,C+=4) //每次同时访问4个数组元素
        {
            float32x4_t neonA = vld1q_f32(A); //依次取4个元素存入寄存器vec
            float32x4_t neonB = vld1q_f32(B); //依次取4个元素存入寄存器vec
            float32x4_t neonC;
            neonC = vaddq_f32(neonA, neonB);//ri = ai + bi 计算两组寄存器对应元素之和并存放到相应结果
            neonC = vaddq_f32(neonC, vsubq_f32(neonA,neonB));
            neonC = vaddq_f32(neonC, vmulq_f32(neonA,neonB));
            vst1q_f32(C,neonC);
        }
        for (; left4>0; left4--,  A++,B++,C++){ //对于剩下的少于4的数字，依次计算累加即可
            *C=*A + *B;
            *C+=*A - *B;
            *C+=*A * *B;
        }
           
        
    }
	
	void VecAddInt32(int *vecA ,int *vecB ,int *vecC,int len){
        int *A=vecA;
        int *B=vecB;
        int *C=vecC;
        for(int i=0;i<len;i++){
            C[i]=A[i]+B[i];//加减乘
            C[i]+=A[i]-B[i];
            C[i]+=A[i]*B[i];
        }
    }

	void VecAddInt32Neon(int *vecA ,int *vecB,int *vecC,int len){
		int *A=vecA;
        int *B=vecB;
        int *C=vecC;
        int dim4= len >> 2; // 数组长度除4整数
        int left4= len & 3; // 数组长度除4余数
        for (; dim4>0; dim4--, A+=4,B+=4,C+=4) //每次同时访问4个数组元素
        {
            int32x4_t neonA = vld1q_s32(A); //依次取4个元素存入寄存器vec
            int32x4_t neonB = vld1q_s32(B); //依次取4个元素存入寄存器vec
            int32x4_t neonC;
            neonC = vaddq_s32(neonA, neonB);//ri = ai + bi 计算两组寄存器对应元素之和并存放到相应结果
            neonC = vaddq_s32(neonC, vsubq_s32(neonA,neonB));
            neonC = vaddq_s32(neonC, vmulq_s32(neonA,neonB));
            vst1q_s32(C,neonC);
        }
        for (; left4>0; left4--,  A++,B++,C++){ //对于剩下的少于4的数字，依次计算累加即可
            *C=*A + *B;
            *C+=*A - *B;
            *C+=*A * *B;
        }
	}
	
	void VecAddUint8(uint8 *vecA ,uint8 *vecB,uint8 *vecC,int len){
        uint8 *A=vecA;
        uint8 *B=vecB;
        uint8 *C=vecC;
        for(int i=0;i<len;i++){
            C[i]=A[i]+B[i];//加减乘
            C[i]+=A[i]-B[i];
            C[i]+=A[i]*B[i];
        }
    }
	
	void VecAddUint8Neon(uint8 *vecA ,uint8 *vecB,uint8 *vecC,int len){
		uint8 *A=vecA;
        uint8 *B=vecB;
        uint8 *C=vecC;
        int dim16= len >> 4; // 数组长度除16整数
        int left16= len % 16; // 数组长度除16余数
        for (; dim16>0; dim16--, A+=16,B+=16,C+=16) //每次同时访问16个数组元素
        {
            uint8x16_t neonA = vld1q_u8(A); //依次取16个元素存入寄存器vec
            uint8x16_t neonB = vld1q_u8(B); //依次取16个元素存入寄存器vec
            uint8x16_t neonC;
            neonC = vaddq_u8(neonA, neonB);//ri = ai + bi 计算两组寄存器对应元素之和并存放到相应结果
            neonC = vaddq_u8(neonC, vsubq_u8(neonA,neonB));
            neonC = vaddq_u8(neonC, vmulq_u8(neonA,neonB));
            vst1q_u8(C,neonC);
        }
        for (; left16>0; left16--,  A++,B++,C++){ //对于剩下的少于16的数字，依次计算累加即可
            *C=*A + *B;
            *C+=*A - *B;
            *C+=*A * *B;
        }
	}
	
	void VecAddUint16(uint16 *vecA ,uint16 *vecB,uint16 *vecC,int len){
        uint16 *A=vecA;
        uint16 *B=vecB;
        uint16 *C=vecC;
        for(int i=0;i<len;i++){
            C[i]=A[i]+B[i];//加减乘
            C[i]+=A[i]-B[i];
            C[i]+=A[i]*B[i];
        }
    }
	
	void VecAddUint16Neon(uint16 *vecA ,uint16 *vecB,uint16 *vecC,int len){
		uint16 *A=vecA;
        uint16 *B=vecB;
        uint16 *C=vecC;
        int dim8= len >> 3; // 数组长度除8整数
        int left8= len % 8; // 数组长度除8余数
        for (; dim8>0; dim8--, A+=8,B+=8,C+=8) //每次同时访问8个数组元素
        {
            uint16x8_t neonA = vld1q_u16(A); //依次取8个元素存入寄存器vec
            uint16x8_t neonB = vld1q_u16(B); //依次取8个元素存入寄存器vec
            uint16x8_t neonC;
            neonC = vaddq_u16(neonA, neonB);//ri = ai + bi 计算两组寄存器对应元素之和并存放到相应结果
            neonC = vaddq_u16(neonC, vsubq_u16(neonA,neonB));
            neonC = vaddq_u16(neonC, vmulq_u16(neonA,neonB));
            vst1q_u16(C,neonC);
        }
        for (; left8>0; left8--,  A++,B++,C++){ //对于剩下的少于8的数字，依次计算累加即可
            *C=*A + *B;
            *C+=*A - *B;
            *C+=*A * *B;
        }
	}
    /*/////////////
    A: M x K
    B: K x N
    C: M x N
    */////////////

    void GemmFloat32( int M, int K, int N, float **GemmA, float **GemmB,float **GemmC){
        for(int i=0;i<M;i++){
            for(int j=0;j<N;j++){
                *(*(GemmC+i)+j)=0;
                for(int m=0;m<K;m++){
                    *(*(GemmC+i)+j)+=*(*(GemmA+i) +m) * *(*(GemmB+m)+j);//与C[i][j]+=A[i][m]*B[m][j]含义相同
                    //GemmC[i][j]+=GemmA[i][m]*GemmB[m][j];
                }
            }
        }
    }

	void GemmFloat32Neon( int M, int K, int N, float **GemmA, float **GemmB,float **GemmC){
        float **A=GemmA;
        float **B=GemmB;
        float **C=GemmC;
        int dimM4= M >> 2;
        int dimN4= N >> 2;
        for(int i=0;i<dimM4;i++){
            for(int j=0;j<dimN4;j++){
                float32x4_t vc0 = vdupq_n_f32(0.0f);
                float32x4_t vc1 = vdupq_n_f32(0.0f);
                float32x4_t vc2 = vdupq_n_f32(0.0f);
                float32x4_t vc3 = vdupq_n_f32(0.0f);

                for (int m = 0; m < K; m++) {
                    float32x4_t vb = vld1q_f32(&B[m][j]);
                    //浮点乘加指令，注意此指令仅仅有f16 f32 f64
                    vc0=vfmaq_f32(vc0, vdupq_n_f32(A[i][m]), vb);//r=c+a*b
                    vc1=vfmaq_f32(vc1, vdupq_n_f32(A[i+1][m]), vb);
                    vc2=vfmaq_f32(vc2, vdupq_n_f32(A[i+2][m]), vb);
                    vc3=vfmaq_f32(vc3, vdupq_n_f32(A[i+3][m]), vb);
                }
                vst1q_f32(&C[i][j],vc0);
                vst1q_f32(&C[i+1][j],vc1);
                vst1q_f32(&C[i+2][j],vc2);
                vst1q_f32(&C[i+3][j],vc3);
            }
        }
        //处理不能被分成4X4 的剩余块
        //三个区域 右上，左下，下拐角(B ,C D 区域)
        /*
        AAAABB
        AAAABB
        AAAABB
        AAAABB
        CCCCDD
        CCCCDD
        CCCCDD
        */
       //C D 区域合并处理
        for(int i=(dimM4<<2);i<M;i++){
            for(int j=0;j<N;j++){
                GemmC[i][j]=0;
                for(int m=0;m<K;m++){
                    *(*(GemmC+i)+j)+=*(*(GemmA+i) +m) * *(*(GemmB+m)+j);//与C[i][j]+=A[i][m]*B[m][j]含义相同
                    //GemmC[i][j]+=GemmA[i][m]*GemmB[m][j];
                }
            }
        }
        //B 区域
        for(int i=0;i<(dimM4<<2);i++){
            for(int j=(dimN4<<2);j<N;j++){
                GemmC[i][j]=0;
                for(int m=0;m<K;m++){
                    *(*(GemmC+i)+j)+=*(*(GemmA+i) +m) * *(*(GemmB+m)+j);//与C[i][j]+=A[i][m]*B[m][j]含义相同
                    //GemmC[i][j]+=GemmA[i][m]*GemmB[m][j];
                }
            }
        }
    }

     /*/////////////
    A: M x K
    B: K x N
    C: M x N
    */////////////

    void GemmUint8( int M, int K, int N, uint8 **GemmA, uint8 **GemmB,uint8 **GemmC){
        for(int i=0;i<M;i++){
            for(int j=0;j<N;j++){
                *(*(GemmC+i)+j)=0;
                for(int m=0;m<K;m++){
                    *(*(GemmC+i)+j)+=*(*(GemmA+i) +m) * *(*(GemmB+m)+j);//与C[i][j]+=A[i][m]*B[m][j]含义相同
                    //GemmC[i][j]+=GemmA[i][m]*GemmB[m][j];
                }
            }
        }
    }

	void GemmUint8Neon( int M, int K, int N, uint8 **GemmA, uint8 **GemmB,uint8 **GemmC){
        uint8 **A=GemmA;
        uint8 **B=GemmB;
        uint8 **C=GemmC;
        int dimM16= M >> 4;
        int dimN16= N >> 4;
        for(int i=0;i<dimM16;i++){
            for(int j=0;j<dimN16;j++){
                uint8x16_t vc[16];//=vdupq_n_u8(1);
                for(int n=0;n<16;n++)
                    vc[n]=vdupq_n_u8(0);

                for (int m = 0; m < K; m++) {
                    uint8x16_t vb = vld1q_u8(&B[m][j]);
                    for(int n=0;n<16;n++){
                        vc[n]=vaddq_u8(vc[n],vmulq_u8(vdupq_n_u8(A[i+n][m]), vb));//r=c+a*b
                    }
                }
                for(int n=0;n<16;n++){
                   vst1q_u8(&C[i+n][j],vc[n]); 
                }
            }
        }
        //处理不能被分成16X16 的剩余块
        //三个区域 右上，左下，下拐角(B ,C D 区域)注意此处是4x4块的示意图
        /*
        AAAABB
        AAAABB
        AAAABB
        AAAABB
        CCCCDD
        CCCCDD
        CCCCDD
        */
       //C D 区域合并处理
        for(int i=(dimM16<<4);i<M;i++){
            for(int j=0;j<N;j++){
                GemmC[i][j]=0;
                for(int m=0;m<K;m++){
                    *(*(GemmC+i)+j)+=*(*(GemmA+i) +m) * *(*(GemmB+m)+j);//与C[i][j]+=A[i][m]*B[m][j]含义相同
                    //GemmC[i][j]+=GemmA[i][m]*GemmB[m][j];
                }
            }
        }
        //B 区域
        for(int i=0;i<(dimM16<<4);i++){
            for(int j=(dimN16<<4);j<N;j++){
                GemmC[i][j]=0;
                for(int m=0;m<K;m++){
                    *(*(GemmC+i)+j)+=*(*(GemmA+i) +m) * *(*(GemmB+m)+j);//与C[i][j]+=A[i][m]*B[m][j]含义相同
                    //GemmC[i][j]+=GemmA[i][m]*GemmB[m][j];
                }
            }
        }
    }
	
	 /*/////////////
    A: M x K
    B: K x N
    C: M x N
    */////////////

    void GemmUint16( int M, int K, int N, uint16 **GemmA, uint16 **GemmB,uint16 **GemmC){
        for(int i=0;i<M;i++){
            for(int j=0;j<N;j++){
                *(*(GemmC+i)+j)=0;
                for(int m=0;m<K;m++){
                    *(*(GemmC+i)+j)+=*(*(GemmA+i) +m) * *(*(GemmB+m)+j);//与C[i][j]+=A[i][m]*B[m][j]含义相同
                    //GemmC[i][j]+=GemmA[i][m]*GemmB[m][j];
                }
            }
        }
    }
	void GemmUint16Neon( int M, int K, int N, uint16 **GemmA, uint16 **GemmB,uint16 **GemmC){
        uint16 **A=GemmA;
        uint16 **B=GemmB;
        uint16 **C=GemmC;
        int dimM8= M >> 3;
        int dimN8= N >> 3;
        for(int i=0;i<dimM8;i++){
            for(int j=0;j<dimN8;j++){
                uint16x8_t vc[8];//=vdupq_n_u16(1);
                for(int n=0;n<8;n++)
                    vc[n]=vdupq_n_u16(0);

                for (int m = 0; m < K; m++) {
                    uint16x8_t vb = vld1q_u16(&B[m][j]);
                    for(int n=0;n<8;n++){
                        vc[n]=vaddq_u16(vc[n],vmulq_u16(vdupq_n_u16(A[i+n][m]), vb));//r=c+a*b
                    }
                }
                for(int n=0;n<8;n++){
                   vst1q_u16(&C[i+n][j],vc[n]); 
                }
            }
        }
        //处理不能被分成8X8 的剩余块
        //三个区域 右上，左下，下拐角(B ,C D 区域)注意此处是4x4块的示意图
        /*
        AAAABB
        AAAABB
        AAAABB
        AAAABB
        CCCCDD
        CCCCDD
        CCCCDD
        */
       //C D 区域合并处理
        for(int i=(dimM8<<3);i<M;i++){
            for(int j=0;j<N;j++){
                GemmC[i][j]=0;
                for(int m=0;m<K;m++){
                    *(*(GemmC+i)+j)+=*(*(GemmA+i) +m) * *(*(GemmB+m)+j);//与C[i][j]+=A[i][m]*B[m][j]含义相同
                    //GemmC[i][j]+=GemmA[i][m]*GemmB[m][j];
                }
            }
        }
        //B 区域
        for(int i=0;i<(dimM8<<3);i++){
            for(int j=(dimN8<<3);j<N;j++){
                GemmC[i][j]=0;
                for(int m=0;m<K;m++){
                    *(*(GemmC+i)+j)+=*(*(GemmA+i) +m) * *(*(GemmB+m)+j);//与C[i][j]+=A[i][m]*B[m][j]含义相同
                    //GemmC[i][j]+=GemmA[i][m]*GemmB[m][j];
                }
            }
        }
    }

	 /*/////////////
    A: M x K
    B: K x N
    C: M x N
    */////////////
	
	void GemmInt32( int M, int K, int N, int **GemmA, int **GemmB,int **GemmC){
        for(int i=0;i<M;i++){
            for(int j=0;j<N;j++){
                *(*(GemmC+i)+j)=0;
                for(int m=0;m<K;m++){
                    *(*(GemmC+i)+j)+=*(*(GemmA+i) +m) * *(*(GemmB+m)+j);//与C[i][j]+=A[i][m]*B[m][j]含义相同
                    //GemmC[i][j]+=GemmA[i][m]*GemmB[m][j];
                }
            }
        }
    }
	void GemmInt32Neon( int M, int K, int N, int **GemmA, int **GemmB,int **GemmC){
        int **A=GemmA;
        int **B=GemmB;
        int **C=GemmC;
        int dimM4= M >> 2;
        int dimN4= N >> 2;
        for(int i=0;i<dimM4;i++){
            for(int j=0;j<dimN4;j++){
                int32x4_t vc[4];//=vdupq_n_s32(1);
                for(int n=0;n<4;n++)
                    vc[n]=vdupq_n_s32(0);

                for (int m = 0; m < K; m++) {
                    int32x4_t vb = vld1q_s32(&B[m][j]); //地址起始的4个int数据到vb
                    for(int n=0;n<4;n++){
                        vc[n]=vaddq_s32(vc[n],vmulq_s32(vdupq_n_s32(A[i+n][m]), vb));//r=c+a*b
                    }
                }
                for(int n=0;n<4;n++){
                   vst1q_s32(&C[i+n][j],vc[n]); 
                }
            }
        }
        //处理不能被分成4X4 的剩余块
        //三个区域 右上，左下，下拐角(B ,C D 区域)注意此处是3x3块的示意图
        /*
        AAABB
        AAABB
        AAABB
        CCCDD
        CCCDD
        */
       //C D 区域合并处理
        for(int i=(dimM4<<2);i<M;i++){
            for(int j=0;j<N;j++){
                GemmC[i][j]=0;
                for(int m=0;m<K;m++){
                    *(*(GemmC+i)+j)+=*(*(GemmA+i) +m) * *(*(GemmB+m)+j);//与C[i][j]+=A[i][m]*B[m][j]含义相同
                    //GemmC[i][j]+=GemmA[i][m]*GemmB[m][j];
                }
            }
        }
        //B 区域
        for(int i=0;i<(dimM4<<2);i++){
            for(int j=(dimN4<<2);j<N;j++){
                GemmC[i][j]=0;
                for(int m=0;m<K;m++){
                    *(*(GemmC+i)+j)+=*(*(GemmA+i) +m) * *(*(GemmB+m)+j);//与C[i][j]+=A[i][m]*B[m][j]含义相同
                    //GemmC[i][j]+=GemmA[i][m]*GemmB[m][j];
                }
            }
        }
    }
	void CopyFloat32(float * copyA,float *copyB,int len){
        for(int i=0;i<len;i++)
            copyB[i]=copyA[i];
    }
    void CopyFloat32Neon(float * copyA,float *copyB,int len){
        float *A=copyA;
        float *B=copyB;
        int dim4=len>>2;
        int left4=len&3;
        for(;dim4>0;dim4--){
            float32x4_t vc=vld1q_f32(A);
            A+=4;
            vst1q_f32(B,vc);
            B+=4;
        }
        for(;left4>0;left4--){
            *B=*A;
        }
        //test vld3的使用
        // float32x4x3_t vc=vld3q_f32(A);
        // float32x4_t vc0=vc.val[0];
        // vst1q_f32(B,vc0);
        // for (int i=0;i<4;i++)
        //     cout<<B[i]<<", ";
        // cout<<endl;

        //test 当访问越界的情况
        // float *newa=new float [5];
        // for(int i=0;i<5;i++)
        //     newa[i]=i;
        // float32x4x3_t v=vld3q_f32(newa);
        // float32x4_t v0=v.val[0];
        // vst1q_f32(newa,v0);
        // for (int i=0;i<4;i++)
        //     cout<<newa[i]<<", ";
        // cout<<endl;

    }
	
	void CopyInt32(int * copyA,int *copyB,int len){
        for(int i=0;i<len;i++)
            copyB[i]=copyA[i];
    }
    void CopyInt32Neon(int * copyA,int *copyB,int len){
        int *A=copyA;
        int *B=copyB;
        int dim4=len>>2;
        int left4=len&3;
        for(;dim4>0;dim4--){
            int32x4_t vc=vld1q_s32(A);
            A+=4;
            vst1q_s32(B,vc);
            B+=4;
        }
        for(;left4>0;left4--){
            *B=*A;
        }
    }
	
	void CopyUint16(uint16 * copyA,uint16 *copyB,int len){
        for(int i=0;i<len;i++)
            copyB[i]=copyA[i];
    }
    void CopyUint16Neon(uint16 * copyA,uint16 *copyB,int len){
        uint16 *A=copyA;
        uint16 *B=copyB;
        int dim8=len>>3;
        int left8=len%(8);
        for(;dim8>0;dim8--){
            uint16x8_t vc=vld1q_u16(A);
            vst1q_u16(B,vc);
            A+=(8);
            B+=(8);
        }
        for(;left8>0;left8--){
            *B=*A;
        }
        cout<<int (copyB[1000000-1])<<endl;
    }

    void CopyUint8(uint8 * copyA,uint8 *copyB,int len){
        for(int i=0;i<len;i++)
            copyB[i]=copyA[i];
    }
    void CopyUint8Neon(uint8 * copyA,uint8 *copyB,int len){
        uint8 *A=copyA;
        uint8 *B=copyB;
        int dim16=len>>4;
        int left16=len%(16);
        for(;dim16>0;dim16--){
            uint8x16_t vc=vld1q_u8(A);
            vst1q_u8(B,vc);
            A+=(16);
            B+=(16);
        }
        for(;left16>0;left16--){
            *B=*A;
        }
        cout<<int (copyB[1000000-1])<<endl;
    }

	void MatrixMulFloat32(  float *vecA, float *vecB,float *vecC){
	//4*4矩阵相乘中，neon算法中，由于寻址的原因，
	//使得矩阵乘计算方式为：vecA的列乘vecB行；
	//没有对矩阵进行转置，故下面算法通过：列乘行实现
	
        for(int i=0;i<4;i++){
            for(int j=0;j<4;j++){
                for(int m=0;m<4;m++){
					//vecC[i*4+j]+=vecA[i*4+m]*vecB[m*4+j];
					*(vecC+4*i+j) += (*(vecB+4*i+m)) * (*(vecA+m*4+j));
                }
            }
        }
    }

	void MatrixMulFloat32Neon( const float *vecA, const float *vecB, float *vecC ){

		float32x4_t a0, a1, a2, a3, b0, b1, b2, b3, r0, r1, r2, r3;
		a0 = vld1q_f32(vecA);  /* col 0 of vecA */
		a1 = vld1q_f32(vecA + 4); /* col 1 of vecA */
		a2 = vld1q_f32(vecA + 8); /* col 2 of vecA */
		a3 = vld1q_f32(vecA + 12); /* col 3 of vecA */

		b0 = vld1q_f32(vecB);  /* col 0 of vecB */
		b1 = vld1q_f32(vecB + 4);  /* col 1 of vecB */
		b2 = vld1q_f32(vecB + 8);  /* col 2 of vecB */
		b3 = vld1q_f32(vecB + 12); /* col 3 of vecB */
		
		/* compute all the cols in the order specified by compiler */
		r0 = vmulq_lane_f32(a0, vget_low_f32(b0), 0);//寄存器a0的每个元素与寄存器b0中的索引为0的元素进行相乘操作，并返回
		r0 = vmlaq_lane_f32(r0, a1, vget_low_f32(b0), 1);//寄存器a1的每个元素与寄存器b0中的索引为1的元素进行相乘操作，再与r中的元素对应相加，并返回
		r0 = vmlaq_lane_f32(r0, a2, vget_high_f32(b0), 0);//vget_high_f32 取高两位
		r0 = vmlaq_lane_f32(r0, a3, vget_high_f32(b0), 1);//以上四步实现：矩阵第一行与第一列的乘积、第二行乘第一列、3行*1列、4行*1列的乘积分别存入r0中
		
		r1 = vmulq_lane_f32(a0, vget_low_f32(b1), 0);
		r1 = vmlaq_lane_f32(r1, a1, vget_low_f32(b1), 1);
		r1 = vmlaq_lane_f32(r1, a2, vget_high_f32(b1), 0);
		r1 = vmlaq_lane_f32(r1, a3, vget_high_f32(b1), 1);
		
		r2 = vmulq_lane_f32(a0, vget_low_f32(b2), 0);
		r2 = vmlaq_lane_f32(r2, a1, vget_low_f32(b2), 1);
		r2 = vmlaq_lane_f32(r2, a2, vget_high_f32(b2), 0);
		r2 = vmlaq_lane_f32(r2, a3, vget_high_f32(b2), 1);
		
		r3 = vmulq_lane_f32(a0, vget_low_f32(b3), 0);
		r3 = vmlaq_lane_f32(r3, a1, vget_low_f32(b3), 1);
		r3 = vmlaq_lane_f32(r3, a2, vget_high_f32(b3), 0);
		r3 = vmlaq_lane_f32(r3, a3, vget_high_f32(b3), 1);
		
		vst1q_f32(vecC, r0);
		vst1q_f32(vecC + 4, r1);
		vst1q_f32(vecC + 8, r2);
		vst1q_f32(vecC + 12, r3);
    }
	
	void CopyFloat32ToUint8(float * copyA,uint8 *copyB,int len){
		for(int i=0;i<len;i++)
            copyB[i]=copyA[i];
	}
	void CopyFloat32ToUint8Neon(float * copyA,uint8 *copyB,int len){
		float *A=copyA;
        uint8 *B=copyB;
        int dim4=len>>3;//一次取8个元素
        int left4=len%8;
        for(;dim4>0;dim4--){
            float32x4_t vc0=vld1q_f32(A);
            A+=4;
			float32x4_t vc1=vld1q_f32(A);
			A+=4;
			uint16x4_t temp0= vqmovn_u32( vcvtq_u32_f32(vc0) );//f32转u32,u32->u16
			uint16x4_t temp1= vqmovn_u32( vcvtq_u32_f32(vc1) );
			uint16x8_t temp=vcombine_u16(temp0,temp1);
			uint8x8_t result=vqmovn_u16(temp);
            vst1_u8(B,result);
            B+=8;
        }
        for(;left4>0;left4--){
            *B=*A;
        }
	}
	void ConvertInt8ToUint16(int8 * copyA,uint16 *copyB,int len){
		for(int i=0;i<len;i++)
            if(copyA[i]<0)
                 copyB[i]=0;
            else
                copyB[i]=copyA[i];
	}
    void ConvertInt8ToUint8(int8 * copyA,uint8 *copyB,int len){
		for(int i=0;i<len;i++)
            if( copyA[i]<0)
                copyB[i]=0;
            else
                copyB[i]=copyA[i];
	}
    void ConvertUint8ToInt8(uint8 * copyA,int8 *copyB,int len){
		for(int i=0;i<len;i++)
            if( copyA[i]>CHAR_MAX)
                copyB[i]=CHAR_MAX;
            else
                copyB[i]=copyA[i];
	}
    void ConvertInt8ToUint8Neon(int8 * copyA,uint8 *copyB,int len){
        int8 *A=copyA;
        uint8 *B=copyB;
        int dim16=len>>4;//取16个数据
        int left16=len%16;
        for(;dim16>0;dim16--){
            int8x16_t temp8x16=vld1q_s8(A);
            int16x8_t temp16x8_l=vmovl_s8(vget_low_s8(temp8x16));
            int16x8_t temp16x8_h=vmovl_s8(vget_high_s8(temp8x16));
            uint8x8_t res8x8_l=vqmovun_s16(temp16x8_l);
            uint8x8_t res8x8_h=vqmovun_s16(temp16x8_h);
            vst1_u8(B,res8x8_l);
            vst1_u8(B+8,res8x8_h);
            A+=16;
            B+=16;
        }
        for(;left16>0;left16--){
            *B=*A;
        }
	}
    void ConvertUint8ToInt8Neon(uint8 * copyA,int8 *copyB,int len){
        uint8 *A=copyA;
        int8 *B=copyB;
        int dim16=len>>4;//取16个数据
        int left16=len%16;
        for(;dim16>0;dim16--){
            uint8x16_t temp8x16=vld1q_u8(A);
            uint16x8_t temp16x8_l=vmovl_u8(vget_low_u8(temp8x16));
            uint16x8_t temp16x8_h=vmovl_u8(vget_high_u8(temp8x16));
            int16x8_t temp16x8_l_int=vreinterpretq_s16_u16(temp16x8_l);
            int16x8_t temp16x8_h_int=vreinterpretq_s16_u16(temp16x8_h);
            int8x8_t res8x8_l=vqmovn_s16(temp16x8_l_int);
            int8x8_t res8x8_h=vqmovn_s16(temp16x8_h_int);
            vst1_s8(B,res8x8_l);
            vst1_s8(B+8,res8x8_h);
            A+=16;
            B+=16;
        }
        for(;left16>0;left16--){
            *B=*A;
        }
	}
	void ConvertInt8ToUint16Neon(int8 * copyA,uint16 *copyB,int len){
		int8 *A=copyA;
        uint16 *B=copyB;
        int dim8=len>>3;//一次取8个元素
        int left8=len%8;
        for(;dim8>0;dim8--){
            
			int8x8_t temp0 = vld1_s8(A);
			int16x8_t temp1 = vmovl_s8(temp0);
			//vget_low_s16取int16x8_t result寄存器中低位四个元素输出int16x4_t，vmovl_s16将int16x4_t转为int32x4_t
			int32x4_t temp10=vmovl_s16( vget_low_s16(temp1));
			int32x4_t temp11=vmovl_s16( vget_high_s16(temp1));
			uint16x4_t result0=vqmovun_s32(temp10);
			uint16x4_t result1=vqmovun_s32(temp11);
			vst1_u16(B,result0);
			vst1_u16(B+4,result1);
			B+=8;
			A+=8;
        }
		cout<<copyB[0]<<endl;
        for(;left8>0;left8--){
            *B=*A;
        }
	}
	void TestVqmovn(){
        int16x8_t a; 
        uint8x8_t b;
        uint8 temp;
        a=vdupq_n_s16(257);
        b=vdup_n_u8(0);
        b=vqmovun_s16(a);
        temp=vget_lane_u8(b,0);
        cout<<"temp:="<<(int)temp<<endl;// 255 

        a=vdupq_n_s16(-4);
        b=vdup_n_u8(0);
        b=vqmovun_s16(a);
        temp=vget_lane_u8(b,0);//0
        cout<<"temp:="<<(int)temp<<endl;// 0

        int16x8_t a1;
        int8x8_t b1;
        signed char temp1;
        a1=vdupq_n_s16(-4);
        b1=vdup_n_s8(1);
        b1=vqmovn_s16(a1);
        temp1=vget_lane_s8(b1,0);//-4
        cout<<"temp1:="<<(int)temp1<<endl;// -4
        {
            int8x16_t a2;
            int8x8_t b2=vdup_n_s8(1);
            int8x8_t c2=vdup_n_s8(2);
            a2=vcombine_s8(b2,c2);
            temp1=vgetq_lane_s8(a2,0);
            cout<<"temp1:="<<(int)temp1<<endl;//1
            temp1=vgetq_lane_s8(a2,8);
            cout<<"temp1:="<<(int)temp1<<endl;//2
        }
        
        {
            int8x16_t x=vreinterpretq_s8_u8(vdupq_n_u8(128));
            temp1=vgetq_lane_s8(x,0);
            cout<<"temp1:="<<(int)temp1<<endl;//1
        }
        
    }
	void TestVcvtf32(){
		float a[4]={2.2,-2.5,-3.5,4.7};
		int b[4];
		float32x4_t temp=vld1q_f32(a);
		int32x4_t result=vcvtq_s32_f32(temp);//四舍五入为0模式，即向0舍入；直接去小数部分
		vst1q_s32(b,result);
		cout<<"float32 -> int32(向0舍入)：";
		for(int i=0;i<4;i++){
			cout<<b[i]<<" ";
		}
		cout<<endl;cout<<endl;
		
		result= vcvtnq_s32_f32(temp);//
		vst1q_s32(b,result);
		cout<<"float32 -> int32(舍入到与偶数相关的最近)：";
		for(int i=0;i<4;i++){
			cout<<b[i]<<" ";
		}
		cout<<endl;cout<<endl;
		
		result= vcvtaq_s32_f32(temp);//四舍五入
		vst1q_s32(b,result);
		cout<<"float32 -> int32(四舍五入)：";
		for(int i=0;i<4;i++){
			cout<<b[i]<<" ";
		}
		cout<<endl;
	}
     ~TestNeon(){
        delete [] arrFloat32;
        delete [] arrInt32;
        delete [] arrUint32;
        delete [] arrUint16;
        delete [] arrUint8;

        delete [] vecAddFloat32_A;
        delete [] vecAddFloat32_B;
        delete [] vecAddFloat32_C;
        delete [] vecAddInt32_A;
        delete [] vecAddInt32_B;
        delete [] vecAddInt32_C;
        delete [] vecAddUint16_A;
        delete [] vecAddUint16_B;
        delete [] vecAddUint16_C;
        delete [] vecAddUint8_A;
        delete [] vecAddUint8_B;
        delete [] vecAddUint8_C;

		delete [] vecMatrixFloat32_A;
		delete [] vecMatrixFloat32_B;
		delete [] vecMatrixFloat32_C;
		
        delete [] arr2DGemmFloat32_A[0];
        delete [] arr2DGemmFloat32_B[0];
        delete [] arr2DGemmFloat32_C[0];
		delete [] arr2DGemmInt32_A[0];
		delete [] arr2DGemmInt32_B[0];
		delete [] arr2DGemmInt32_C[0];
		delete [] arr2DGemmUint16_A[0];
		delete [] arr2DGemmUint16_B[0];
		delete [] arr2DGemmUint16_C[0];
        delete [] arr2DGemmUint8_A[0];
		delete [] arr2DGemmUint8_B[0];
		delete [] arr2DGemmUint8_C[0];
        delete [] arr2DGemmFloat32_A;
        delete [] arr2DGemmFloat32_B;
        delete [] arr2DGemmFloat32_C;
		delete [] arr2DGemmInt32_A;
		delete [] arr2DGemmInt32_B;
		delete [] arr2DGemmInt32_C;
		delete [] arr2DGemmUint16_A;
		delete [] arr2DGemmUint16_B;
		delete [] arr2DGemmUint16_C;
        delete [] arr2DGemmUint8_A;
		delete [] arr2DGemmUint8_B;
		delete [] arr2DGemmUint8_C;

        delete [] copyFloat32_A;
        delete [] copyInt32_A;
		delete [] copyInt16_A;
		delete [] copyInt8_A;
		delete [] copyUint32_A;
        delete [] copyUint16_A;
        delete [] copyUint8_A;
        delete [] copyFloat32_B;
        delete [] copyInt32_B;
		delete [] copyInt16_B;
		delete [] copyInt8_B;
		delete [] copyUint32_B;
        delete [] copyUint16_B;
        delete [] copyUint8_B;

    }
private:
        int dataLen;
        //arrSum
        float *arrFloat32;
        int *arrInt32;
        uint32 *arrUint32;
        uint16 *arrUint16;
        uint8 *arrUint8;
        //vecAdd
        float *vecAddFloat32_A;
        float *vecAddFloat32_B;
        float *vecAddFloat32_C;
        int *vecAddInt32_A;
        int *vecAddInt32_B;
        int *vecAddInt32_C;
        uint16 *vecAddUint16_A;
        uint16 *vecAddUint16_B;
        uint16 *vecAddUint16_C;
        uint8 *vecAddUint8_A;
        uint8 *vecAddUint8_B;
        uint8 *vecAddUint8_C;
		//matrixA mult
		float  *vecMatrixFloat32_A;
		float  *vecMatrixFloat32_B;
		float  *vecMatrixFloat32_C;
        //arr2DGemm
        int gemmM;
        int gemmK;
        int gemmN;
        float **arr2DGemmFloat32_A;
        float **arr2DGemmFloat32_B;
        float **arr2DGemmFloat32_C;
        int **arr2DGemmInt32_A;
        int **arr2DGemmInt32_B;
        int **arr2DGemmInt32_C;
        uint16 **arr2DGemmUint16_A;
        uint16 **arr2DGemmUint16_B;
        uint16 **arr2DGemmUint16_C;
        uint8 **arr2DGemmUint8_A;
        uint8 **arr2DGemmUint8_B;
        uint8 **arr2DGemmUint8_C;
        //copy操作
        float *copyFloat32_A;
        float *copyFloat32_B;
        int *copyInt32_A;
        int *copyInt32_B;
		int16 *copyInt16_A;
        int16 *copyInt16_B;
		int8 *copyInt8_A;
		int8 *copyInt8_B;
		uint32 *copyUint32_A;
        uint32 *copyUint32_B;
        uint16 *copyUint16_A;
        uint16 *copyUint16_B;
        uint8 *copyUint8_A;
        uint8 *copyUint8_B;






};
int main(){
   TestNeon* test = new TestNeon();
    test->TestMain();
	delete test;
 	return 0;
}
 
