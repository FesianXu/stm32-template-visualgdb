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
	unsigned char blockLoc ; // 占buf的块的绝对位置，0开始计算
	short validData ; // 块内有效数据数量
	struct BufferCell *ptr_next ; // 下一个块的地址
} bufNode ;

class queue_buf
{
private:
	bufNode head ; // 首部，总是存在一个首部，其内存不会释放。
	int instance_id ; // 实例的编码id
	int length_bufBlock ; // buf的块数
	int queue_buf_head ; // buf的队列头部，入队列从此处开始
	int queue_buf_tail ; // buf的队列尾部,出队列从此处开始
	int length_buf ; // buf的总容量，指的是以字节为单位

	int getValidDataLength(void) ; // 获得buf中有效数据的长度
public:
	queue_buf(int numBlock) ;
	explicit queue_buf(queue_buf &cpy) ; // 深拷贝
	~queue_buf(void) ;
	_API_CPP_BUF_ int pushSingleDataIntoBuffer(const char data) ;// 将data入buffer
	_API_CPP_BUF_ int pushSingleDataIntoBufferFORCE(const char data) ;// 将data入buffer,如果buffer大小不足则自动补充，除非内存不足，否则都可以入buffer
	_API_CPP_BUF_ int pushMultipleDataIntoBuffer(const char *data) ; // 将一组data入buffer，如果buffer大小不够则失败
	_API_CPP_BUF_ int pushMultipleDataIntoBufferFORCE(const char *data); // 将一组data入buffer，如果buffer大小不够则自动补充，除非内存不足，否则都可以入buffer
	_API_CPP_BUF_ char popSingleDataFromBuffer(void) ; // 从buffer弹出单个data
	_API_CPP_BUF_ int printBuf(void) ; // 打印出buffer中的数据
	_API_CPP_BUF_ int resize(int newlen) ; // 重设buffer的大小
	_API_CPP_BUF_ int freshBuf(void) ; // 将buffer中的数据部分清零
	_API_CPP_BUF_ char *turn2String(void) ; // 转变为字符串，需要自行管理内存
	

	_OUT_CPP_BUF_ int getTotalBlockNumber(void) ;
	_OUT_CPP_BUF_ int getBufQueueHeadLoc(void) ;
	_OUT_CPP_BUF_ int getBufQueueTailLoc(void) ;
	_OUT_CPP_BUF_ int getTotalBytesNumber(void) ;
};


#endif