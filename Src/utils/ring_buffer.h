/**
 * @file ring_buffer.h
 * @brief Ring buffer utility for audio/video data buffering
 */

#ifndef RING_BUFFER_H
#define RING_BUFFER_H

typedef struct RING_BUFF
{
    unsigned char *buf;
    unsigned int head;
    unsigned int tail;
    unsigned int tatol_len;  // 保持原来的拼写（total）
} RING_BUFF;

/**
 * @brief Write data to ring buffer
 * @param ring_buf Ring buffer structure
 * @param input_data Input data pointer
 * @param len Data length
 * @return 0 on success, -1 on error
 */
int write_ring_buff(RING_BUFF *ring_buf, unsigned char *input_data, unsigned int len);

/**
 * @brief Read data from ring buffer
 * @param ring_buf Ring buffer structure
 * @param output_data Output data pointer
 * @param len Data length to read
 * @return 0 on success, -1 on error
 */
int read_ring_buff(RING_BUFF *ring_buf, unsigned char *output_data, unsigned int len);

#endif /* RING_BUFFER_H */
