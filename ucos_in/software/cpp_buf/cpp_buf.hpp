/**
* @author: FesianXu
* @date: 2016/12/22
* @platform: WIN7 & VisualStudio 2012
* @description: buffer interface.
*/
#ifndef _CPP_BUFFER_HPP_
#define _CPP_BUFFER_HPP_

#include <cstring>
#include <cstdlib>

#define _API_CPP_BUF_
#define _OUT_CPP_BUF_
#define _LENGTH_BUFFER_CELL_ 200


typedef struct BufferCell
{
	char buf[_LENGTH_BUFFER_CELL_] ;
	unsigned char blockLoc ; // ռbuf�Ŀ�ľ���λ�ã�0��ʼ����
	short validData ; // ������Ч��������
	struct BufferCell *ptr_next ; // ��һ����ĵ�ַ
} bufNode ;

class queue_buf
{
private:
	bufNode head ; // �ײ������Ǵ���һ���ײ������ڴ治���ͷš�
	int instance_id ; // ʵ���ı���id
	int length_bufBlock ; // buf�Ŀ���
	int queue_buf_head ; // buf�Ķ���ͷ��������дӴ˴���ʼ
	int queue_buf_tail ; // buf�Ķ���β��,�����дӴ˴���ʼ
	int length_buf ; // buf����������ָ�������ֽ�Ϊ��λ

	int getValidDataLength(void) ; // ���buf����Ч���ݵĳ���
public:
	queue_buf(int numBlock) ;
	explicit queue_buf(queue_buf &cpy) ; // ���
	~queue_buf(void) ;
	_API_CPP_BUF_ int pushSingleDataIntoBuffer(const char data) ;// ��data��buffer
	_API_CPP_BUF_ int pushSingleDataIntoBufferFORCE(const char data) ;// ��data��buffer,���buffer��С�������Զ����䣬�����ڴ治�㣬���򶼿�����buffer
	_API_CPP_BUF_ int pushMultipleDataIntoBuffer(const char *data) ; // ��һ��data��buffer�����buffer��С������ʧ��
	_API_CPP_BUF_ int pushMultipleDataIntoBufferFORCE(const char *data); // ��һ��data��buffer�����buffer��С�������Զ����䣬�����ڴ治�㣬���򶼿�����buffer
	_API_CPP_BUF_ char popSingleDataFromBuffer(void) ; // ��buffer��������data
	_API_CPP_BUF_ int printBuf(void) ; // ��ӡ��buffer�е�����
	_API_CPP_BUF_ int resize(int newlen) ; // ����buffer�Ĵ�С
	_API_CPP_BUF_ int freshBuf(void) ; // ��buffer�е����ݲ�������
	_API_CPP_BUF_ char *turn2String(void) ; // ת��Ϊ�ַ�������Ҫ���й����ڴ�
	

	_OUT_CPP_BUF_ int getTotalBlockNumber(void) ;
	_OUT_CPP_BUF_ int getBufQueueHeadLoc(void) ;
	_OUT_CPP_BUF_ int getBufQueueTailLoc(void) ;
	_OUT_CPP_BUF_ int getTotalBytesNumber(void) ;
};


#endif