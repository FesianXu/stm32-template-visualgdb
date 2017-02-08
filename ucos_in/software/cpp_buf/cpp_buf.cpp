#include "cpp_buf.hpp"
static int queue_buf_instance_id = 0 ; // queue_buf类的实例全局编码id变量

queue_buf::queue_buf(int numBlock)
{
	queue_buf_instance_id++ ;
	instance_id = queue_buf_instance_id ;
	bufNode *tmp_ptr = NULL ;
	bufNode *tmp_head = NULL ;
	this->head.blockLoc = 0 ;
	this->head.validData = -1 ;
	this->length_bufBlock = numBlock ;
	this->queue_buf_head = 0 ;
	this->queue_buf_tail = 0 ;
	this->length_buf = _LENGTH_BUFFER_CELL_*length_bufBlock ;
	memset(this->head.buf, 0x00, _LENGTH_BUFFER_CELL_*sizeof(char)) ;
	this->head.ptr_next = NULL ;
	tmp_head = &this->head ;
	
	for(int i = 0; i < numBlock-1; i++)
	{
		tmp_ptr = (bufNode *)malloc(sizeof(bufNode)) ;
		tmp_ptr->blockLoc = i+1 ;
		tmp_ptr->validData = -1 ;
		memset(tmp_ptr->buf, 0x00, _LENGTH_BUFFER_CELL_*sizeof(char)) ;
		tmp_ptr->ptr_next = NULL ;
		tmp_head->ptr_next = tmp_ptr ;
		tmp_head = tmp_ptr ;
	}
	tmp_head->ptr_next = &this->head  ;
}

/*

*/
queue_buf::queue_buf(queue_buf &cpy)
{
	queue_buf_instance_id++ ;
	instance_id = queue_buf_instance_id ;
	length_bufBlock = cpy.length_bufBlock ;
	queue_buf_head = cpy.queue_buf_head ;
	queue_buf_tail = cpy.queue_buf_tail ;
	length_buf = cpy.length_buf ;
	// 属性copy
	bufNode *tmp_ptr = NULL ;
	bufNode *tmp_head = &head ;
	bufNode *tmp_cpy = &cpy.head ;
	head.ptr_next = NULL ;
	head.blockLoc = cpy.head.blockLoc ;
	head.validData = cpy.head.validData ;
	memcpy(head.buf, cpy.head.buf, _LENGTH_BUFFER_CELL_*sizeof(char)) ;
	tmp_cpy = tmp_cpy->ptr_next ;
	for(int i = 0; i < length_bufBlock-1; i++)
	{
		tmp_ptr = (bufNode *)malloc(sizeof(bufNode)) ;
		tmp_ptr->blockLoc = tmp_cpy->blockLoc ;
		tmp_ptr->validData = tmp_cpy->validData ;
		memcpy(tmp_ptr->buf, tmp_cpy->buf, _LENGTH_BUFFER_CELL_*sizeof(char)) ;
		tmp_ptr->ptr_next = NULL ;
		tmp_head->ptr_next = tmp_ptr ;
		tmp_head = tmp_ptr ;
		tmp_cpy = tmp_cpy->ptr_next ;
	}
	tmp_head->ptr_next = &head ; // 深拷贝
}

queue_buf::~queue_buf()
{
	bufNode *tmp_delete_now = head.ptr_next ;
	bufNode *tmp_delete_next = tmp_delete_now->ptr_next ;
	for(int i = 0; i < length_bufBlock-1; i++)
	{
		if(tmp_delete_now == NULL)
			continue ;
		free(tmp_delete_now) ;
		tmp_delete_now = tmp_delete_next ;
		tmp_delete_next = tmp_delete_now->ptr_next ;
	}
	head.ptr_next = NULL ;
}

_API_CPP_BUF_ int queue_buf::pushSingleDataIntoBuffer(const char data)
{
	int block_bias = 0 ;
	int byte_bias = 0 ;
	bufNode *tmp_head = &this->head ;
	if((queue_buf_head+1)%length_buf == queue_buf_tail)
		return -1 ;
	queue_buf_head = (queue_buf_head+1)%length_buf ;
	block_bias = queue_buf_head/_LENGTH_BUFFER_CELL_ ;
	byte_bias = queue_buf_head%_LENGTH_BUFFER_CELL_ ;
	for(int i = 0; i < block_bias; i++)
		tmp_head = tmp_head->ptr_next ;
	tmp_head->buf[byte_bias] = data ;
	tmp_head->validData++ ;
	return 0 ;
}

/************************************************************************/
/* 在串口中多次会重复resize，导致内存泄漏，还需要调试                    
/************************************************************************/
_API_CPP_BUF_ int queue_buf::pushSingleDataIntoBufferFORCE(const char data)
{
	int block_bias = 0 ;
	int byte_bias = 0 ;
	bufNode *tmp_head = &head ;
	if((queue_buf_head+1)%length_buf == queue_buf_tail)
		resize(length_bufBlock+1) ; // 如果空间不足，自动补充
	queue_buf_head = (queue_buf_head+1)%length_buf ;
	block_bias = queue_buf_head/_LENGTH_BUFFER_CELL_ ;
	byte_bias = queue_buf_head%_LENGTH_BUFFER_CELL_ ;
	for(int i = 0; i < block_bias; i++)
		tmp_head = tmp_head->ptr_next ;
	tmp_head->buf[byte_bias] = data ;
	tmp_head->validData++ ;
	return 0 ;
} 

_API_CPP_BUF_ int queue_buf::pushMultipleDataIntoBuffer(const char *data)
{
	int block_bias = 0 ;
	int byte_bias = 0 ;
	int len = 0;
	int sum_byte = 0 ;
	len = strlen(data) ;
	sum_byte = getValidDataLength() ;
	if(len > length_buf-1-sum_byte)
		return -1 ;
	for(int i = 0; i < len; i++)
		if(pushSingleDataIntoBuffer(data[i]) != 0)
			return -1 ;
	return 0 ;
}

_API_CPP_BUF_ int queue_buf::pushMultipleDataIntoBufferFORCE(const char *data)
{
	int len = strlen(data) ;
	int sum_byte = getValidDataLength() ;
	if(len > length_buf-1-sum_byte)
	{
		int delta = len-length_buf+sum_byte+1 ;
		resize((int)(delta/_LENGTH_BUFFER_CELL_)+1+length_bufBlock) ;
		pushMultipleDataIntoBuffer(data) ;
	}
	else
		for(int i = 0; i < len; i++)
			if(pushSingleDataIntoBuffer(data[i]) != 0)
				return -1 ;
	return 0 ;
}

_API_CPP_BUF_ char queue_buf::popSingleDataFromBuffer(void)
{
	char tmp_data = 0 ;
	int block_bias = 0 ;
	int byte_bias = 0 ;
	bufNode *tmp_head = NULL ;
	if(queue_buf_head == queue_buf_tail)
		return -1 ;
	queue_buf_tail = (queue_buf_tail+1)%length_buf ;
	tmp_head = &head ;
	block_bias = queue_buf_tail/_LENGTH_BUFFER_CELL_ ;
	byte_bias = queue_buf_tail%_LENGTH_BUFFER_CELL_ ;
	for(int i = 0;i < block_bias; i++)
		tmp_head = tmp_head->ptr_next ;
	tmp_data = tmp_head->buf[byte_bias] ;
	tmp_head->buf[byte_bias] = 0 ;
	tmp_head->validData-- ;
	return tmp_data ;
}

_API_CPP_BUF_ int queue_buf::printBuf(void)
{
	bufNode *tmp_head = &head ;
	int block_bias = 0 ;
	int byte_bias = 0 ;
	int sum_byte = getValidDataLength(); 
	int tmp_queue_tail = queue_buf_tail ;
	for(int i = 0;i < sum_byte; i++)
	{
		tmp_queue_tail = (tmp_queue_tail+1)%length_buf ;
		block_bias = tmp_queue_tail/_LENGTH_BUFFER_CELL_ ;
		byte_bias = tmp_queue_tail%_LENGTH_BUFFER_CELL_ ;
		for(int j = 0;j < block_bias; j++)
			tmp_head = tmp_head->ptr_next ;
		//printf("%c", tmp_head->buf[byte_bias]) ;
		tmp_head = &head ;
	}
	//printf("\r\n") ;
	return 0 ;
}

/**
* 必须自行管理返回的字符串指针，否则会出现内存泄漏，注意
*/
_API_CPP_BUF_ char *queue_buf::turn2String(void)
{
	if(&head == NULL)
		return NULL ; // 传入的字符串没有内容，地址为空，直接返回，用于其他类调用时候
	char *str = NULL ;
	bufNode *tmp_head = &head ;
	int len = getValidDataLength() ;
	int tmp_queue_tail = queue_buf_tail ;
	int block_bias = 0 ;
	int byte_bias = 0 ;
	if(len == 0)
		return NULL ; // 没有内容 直接返回
	str = (char *)malloc(sizeof(char)*(len+1)) ;
	memset(str, 0x00, sizeof(char)*(len+1)) ;
	for(int i = 0; i < len; i++)
	{
		tmp_queue_tail = (tmp_queue_tail+1)%length_buf ;
		block_bias = tmp_queue_tail/_LENGTH_BUFFER_CELL_ ;
		byte_bias = tmp_queue_tail%_LENGTH_BUFFER_CELL_ ;
		for(int j = 0; j < block_bias; j++)
			tmp_head = tmp_head->ptr_next ;
		str[i] = tmp_head->buf[byte_bias] ;
		tmp_head = &head ;
	}
	return str ; // buffer上存在内容，返回字符串堆区上的指针
}

_API_CPP_BUF_ int queue_buf::resize(int newlen)
{
	if(newlen > length_bufBlock)
	{
		int delta = newlen-length_bufBlock ;
		int block_bias = 0 ;
		bufNode *tmpnew = NULL ;
		bufNode *tmp_head = &head ;
		block_bias = queue_buf_head/_LENGTH_BUFFER_CELL_ ;
		for(int i = 0; i < delta; i++)
		{
			tmpnew = (bufNode *)malloc(sizeof(bufNode)) ;
			tmpnew->blockLoc = length_bufBlock ;
			tmpnew->ptr_next = NULL ;
			tmpnew->validData = -1 ;
			memset(tmpnew->buf, 0x00, _LENGTH_BUFFER_CELL_*sizeof(char)) ;
			for(int j = 0;j < block_bias; j++) 
				tmp_head = tmp_head->ptr_next ;
			tmpnew->ptr_next = tmp_head->ptr_next ;
			tmp_head->ptr_next = tmpnew ;
			tmp_head = &head ;
			length_bufBlock++ ;
			block_bias++ ;
		} 
		length_buf = newlen*_LENGTH_BUFFER_CELL_ ;
		return 1 ;
	} // 不管是否已满都可以插入新块，在head前插入
	else if(newlen < length_bufBlock)
	{
		int delta = length_bufBlock-newlen ;
		int usedBlock = (getValidDataLength()/_LENGTH_BUFFER_CELL_)+1 ;
		if(newlen < usedBlock)
			return -1 ; // 不能删除块，已经存在数据了
		bufNode *tmp_head = &head ;
		bufNode *tmp_delete = NULL ;
		int block_bias = queue_buf_head/_LENGTH_BUFFER_CELL_ ;
		for(int i = 0;i < delta; i++)
		{
			for(int j = 0; j < block_bias+i; j++)
				tmp_head = tmp_head->ptr_next ;
			tmp_delete = tmp_head->ptr_next ;
			if(tmp_delete->blockLoc == 0)
				continue ; // 此处指向了buffer头部，不能释放，为栈区数据
			tmp_head->ptr_next = tmp_delete->ptr_next ;
			tmp_delete->ptr_next = NULL ;
			free(tmp_delete) ;
			length_bufBlock-- ;
			tmp_head = &head ;
		} 
		length_buf = newlen*_LENGTH_BUFFER_CELL_ ;
		return 2 ;
	} // 不一定缩小resize之后就是指定的大小，可能多一个头部。
	else if(newlen == length_bufBlock)
		return 0 ;
}

_API_CPP_BUF_ int queue_buf::freshBuf(void)
{
	bufNode *tmp_head = &head ;
	for(int i = 0; i < length_bufBlock; i++)
	{
		memset(tmp_head->buf, 0x00, _LENGTH_BUFFER_CELL_*sizeof(char)) ;
		tmp_head->validData = -1 ;
		tmp_head = tmp_head->ptr_next ;
	}
	queue_buf_head = 0 ;
	queue_buf_tail = 0 ;
	return 0 ;
}

_OUT_CPP_BUF_ int queue_buf::getTotalBlockNumber(void)
{
	return length_bufBlock ;
}

_OUT_CPP_BUF_ int queue_buf::getBufQueueHeadLoc(void)
{
	return queue_buf_head ;
}

_OUT_CPP_BUF_ int queue_buf::getBufQueueTailLoc(void)
{
	return queue_buf_tail ;
}

_OUT_CPP_BUF_ int queue_buf::getTotalBytesNumber(void)
{
	return length_buf ;
}

int queue_buf::getValidDataLength(void)
{
	bufNode *tmp_head = &head ;
	int sum = 0 ;
	for(int i = 0; i < length_bufBlock; i++)
	{
		if(tmp_head->validData == -1)
		{
			tmp_head = tmp_head->ptr_next ;
			continue ;
		}
		sum += (tmp_head->validData+1) ;
		tmp_head = tmp_head->ptr_next ;
	}
	return sum ;
}
