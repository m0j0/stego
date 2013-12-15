#ifndef BIT_STREAM_H
#define BIT_STREAM_H

#include <stdio.h>
#include <string.h>
#include <string>

struct stegosystem
{
	std::string crypto_key;
	std::string stego_key;
	std::string stego_data;
	int stego_size;
};

class bit_stream
{
public:
	bit_stream(void *data, size_t _size)
	{
		size = _size + 4;
		buf = new unsigned char [size];
		memcpy(buf, &_size, sizeof(unsigned int));
		memcpy(buf + sizeof(unsigned int), data, _size);
		pos = 0;
		size_bits = size*8;
		//printf("BITS: %d %d %s\n", size_bits,size,data);
	}

	bit_stream()
	{
		buf = new unsigned char [sizeof(unsigned int)];
		memset(buf, 0, 4);
		pos = 0;
		size_bits = sizeof(unsigned int) * 8;
		size_readed = false;
	}

	int get_readed_size()
	{
		return readed_size;
	}

	void get_data(void *data)
	{
		memset(data, 0, readed_size);
		memcpy(data, buf, readed_size);
	}

	~bit_stream()
	{
		delete [] buf;
	}

	int get_next_bit()
	{		
		return get_bit(pos++);
	}

	int get_next_bit_rotate()
	{		
		int ret = get_bit(pos++);
		if(pos == size_bits)
		{
			//printf("rotate\n");
			pos = 0;
		}
		return ret;
	}

	int get_bit(size_t _pos)
	{
		 if(_pos >= size_bits)
		{
			//printf("END!\n");
			return -1; 
		}

		size_t byte_pos = _pos / 8;
		size_t bit_pos = 7 - _pos % 8;
		//printf("-%c-\n", buf[byte_pos]);
//		printf("%02X, byte: %d, bit: %d --- val: %d\n", (unsigned char)buf[byte_pos], byte_pos,bit_pos, buf[byte_pos] & (1 << bit_pos));

	//	printf(" **** %d %d %d %c\n", byte_pos, bit_pos, !!(buf[byte_pos] & (1 << bit_pos)), buf[byte_pos]);
		return !!(buf[byte_pos] & (1 << bit_pos));
	}

	int set_next_bit(int bit)
	{
		int ret = set_bit(pos++, bit);
		
		if(ret == -1 && !size_readed)
		{
			readed_size = *(unsigned int *)buf;
			delete [] buf;

			buf = new unsigned char [readed_size];
			size_bits = readed_size * 8;
			size_readed = true;
			pos = 0;
			return 0;
		}
		return ret;
	}

	int set_bit(size_t _pos, int bit)
	{
		if(_pos >= size_bits && size_readed == false)
		{
			readed_size = *(unsigned int *)buf;
			delete [] buf;

			buf = new unsigned char [readed_size];
			memset(buf, 0, readed_size);
			size_bits = readed_size * 8;
			size_readed = true;
			pos = _pos = 0;
			pos++;
		}

		if(_pos >= size_bits)
		{
		//	printf("END! %d %d\n",_pos, size_bits);

			return -1; 
		}
		//printf("%s\n", buf);
		//printf("pos: %d\n",_pos);
		
		size_t byte_pos = _pos / 8;
		size_t bit_pos = 7 - _pos % 8;
		buf[byte_pos] |= (bit << bit_pos);
	//	printf(" **** %d %d %d %c %02X\n", byte_pos, bit_pos, bit, buf[byte_pos],buf[byte_pos]);
		


//		printf("%02X, byte: %d, bit: %d --- val: %d\n", (unsigned char)buf[byte_pos], byte_pos,bit_pos, buf[byte_pos] & (1 << bit_pos));
//		return !!(buf[byte_pos] & (1 << bit_pos));
		return 0;
	}

	static void write_bit(char &ch, char bit)
	{
		ch = (ch & 0xFE) + bit;
	}
	
	static void write_bit(short &ch, char bit)
	{
		ch = (ch & 0xFFFE) + bit;
	}

	static char read_bit(char ch)
	{
		return (ch & 1);
	}

private:
	size_t pos;
	size_t size;
	size_t size_bits;
	bool size_readed;
	unsigned int readed_size;
	unsigned char *buf;

};


#endif