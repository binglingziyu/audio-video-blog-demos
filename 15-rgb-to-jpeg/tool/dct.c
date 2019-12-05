
#include<math.h>
#define PI 3.1415926535

int MAT_SIZE = 8;

float DCT_Mat[64]; //定于变换矩阵
float DctMapTmp[64];  //矩阵运算时用的中间矩阵

void transMat() {
    int i,j;
    float a;
    for(i = 0; i < MAT_SIZE; i++) {
        for(j = 0; j < MAT_SIZE; j++) {
            a = 0;
            if(i == 0) {
                a = sqrt((float)1/MAT_SIZE);
            } else {
                a = sqrt((float)2/MAT_SIZE);
            }
            DCT_Mat[i*MAT_SIZE+j] = a*cos((j+0.5)*PI*i/MAT_SIZE); //变换矩阵
        }
    }
}

void fdct(float data[64]) {
    float t=0;
    int i,j,k;
    for(i = 0; i< MAT_SIZE; i++) { //相当于A*I
        for(j = 0; j < MAT_SIZE; j++) {
            t=0;
            for(k = 0; k < MAT_SIZE; k++) {
                t+=DCT_Mat[i*MAT_SIZE+k]*data[k*MAT_SIZE+j]; //矩阵的乘法，DCT_Mat的第i行乘DctMap的第j列
            }
            DctMapTmp[i*MAT_SIZE+j]=t;
        }
    }
    for(i = 0; i < MAT_SIZE; i++) { //相当于（A*I）后再*A‘
        for(j = 0; j < MAT_SIZE; j++) {
            t=0;
            for(k = 0; k < MAT_SIZE; k++) {
                t+=DctMapTmp[i*MAT_SIZE+k]*DCT_Mat[j*MAT_SIZE+k];
            }
            data[i*MAT_SIZE+j]=t;
        }
    }
}

//int main(int argc, char *argv[])
//{
//    MAT_SIZE = atoi(argv[1]); //定义矩阵维度
//    float k = 1;
//    int count = 1;
//    for(int i=0;i<MAT_SIZE;i++)   //产生输入数据
//    {
//        for(int j=0;j<MAT_SIZE;j++)
//        {
//            DctMap[i][j]=k + 50*cos(k*2*PI/40);
//            k++;
//        }
//    }
//    TransMat();
//    DCT();
//    for(int i=0;i<MAT_SIZE;i++)
//    {
//        for(int j=0;j<MAT_SIZE;j++)
//        {
//            printf("%d\t%f\n",count,DctMap[i][j]); //输出DCT变换结果
//            count++;
//        }
//    }
//}
//————————————————
//版权声明：本文为CSDN博主「杨贵安」的原创文章，遵循 CC 4.0 BY-SA 版权协议，转载请附上原文出处链接及本声明。
//原文链接：https://blog.csdn.net/yga_airspace/article/details/86688327