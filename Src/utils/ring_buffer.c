/**
 * @file ring_buffer.c
 * @brief Ring buffer utility implementation
 *
 * Extracted from td_common.c for reusability
 */

#include "ring_buffer.h"
#include <string.h>
#include <stdio.h>
#include "PrintGrade.h"

int write_ring_buff(RING_BUFF *ring_buf, unsigned char *input_data, unsigned int len)
{
	unsigned int t_w = 0;

	if (len > ring_buf->tatol_len || 0 == len || NULL == ring_buf->buf)
	{
		PRINT_ERROR("write_ring_buff error tatol_len = %d len = %d\n",ring_buf->tatol_len,len);
		return -1;
	}

	if (ring_buf->tail == ring_buf->head)
	{
		ring_buf->tail = 0;
		ring_buf->head = 0;
	}

	t_w = ring_buf->tail & (ring_buf->tatol_len - 1);

	if (len <= (ring_buf->tatol_len - t_w))
	{
		memcpy(ring_buf->buf + t_w, input_data, len);
	}
	else
	{
		memcpy(ring_buf->buf + t_w, input_data, (ring_buf->tatol_len - t_w));
		memcpy(ring_buf->buf, input_data + (ring_buf->tatol_len - t_w), len - (ring_buf->tatol_len - t_w));
	}

	ring_buf->tail += len;

	return 0;
}

int read_ring_buff(RING_BUFF *ring_buf, unsigned char *output_data, unsigned int len)
{
	//unsigned int t_w = 0;
	unsigned int t_r = 0;
	unsigned int cur_w = 0;
	unsigned int datalen = 0;
	cur_w = ring_buf->tail;
	datalen = cur_w - ring_buf->head;

	//空了或小于
	if (len > ring_buf->tatol_len || 0 == datalen || datalen < len || 0 == len || NULL == ring_buf->buf)
	{
		return -1;
	}

	//索引在数组的位置
	//t_w = cur_w & (ring_buf->tatol_len - 1);
	t_r = ring_buf->head & (ring_buf->tatol_len - 1);

	if (len <= (ring_buf->tatol_len - t_r))
	{
		memcpy(output_data, ring_buf->buf + t_r, len);
	}
	else
	{
		memcpy(output_data, ring_buf->buf + t_r, (ring_buf->tatol_len - t_r));
		memcpy(output_data + (ring_buf->tatol_len - t_r), ring_buf->buf, len - (ring_buf->tatol_len - t_r));
	}

	ring_buf->head += len;

	return 0;
}
