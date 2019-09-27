/*
 * @brief
 * @author li yong (DogLee)
 * @email 20162439@qq.com
 * @version 1.0
 * @date 2018-08-11 14:55:32
 */
#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <QMutex>
#include <QDebug>
//#include "logmanager.h"

template <typename Type>
class RingBuffer
{
public:
    RingBuffer(int size);

    ~RingBuffer();

    /**
     * @brief read 从缓冲区中读取数据，该函数会清除有效数据个数，read之后，size（）返回值为0.
     * @param pData 数据存放的位置
     * @param size 期望读取的数据长度
     * @return 实际读取到的数据长度
     */
    int read(Type *pData, int size);

    int readOne(Type *pData, int size);

    /**
     * @brief peek 从缓冲区中获取数据，但是不清除有效数据，不会影响size（）的返回值。
     * @param pData 数据存储地址
     * @param size 期望的数据个数
     * @param pos 保存上一次读取到的地址，用于连续读取
     * 如果pos等于-1,表示外面不知道当前的位置索引,所以读取尽量多的数据.读取的长度不大于指定的长度和本缓存的实际长度
     * @return 实际读取数据的个数
     */
    int peek(Type *pData, int size, int& pos);

    /**
     * @brief write 向缓冲区写入数据
     * @param pData 数据源地址
     * @param size 期望写入的数据个数
     * @return 实际写入数据个数
     */
    int write(Type *pData, int size);

    /**
     * @brief empty 判断缓冲区是否为空
     * @return 如果时为缓冲区为空，则返回 \c true,否则返回 \c false
     */
    inline bool empty()
    {
        bool isEmpty = true;

        mutex.lock();
        if (m_counts == 0)
            isEmpty = true;
        else
            isEmpty = false;
        mutex.unlock();

        return isEmpty;
    }

    /**
     * @brief empty 判断缓冲区是否已满
     * @return 如果时为缓冲区已满，则返回 \c true,否则返回 \c false
     */
    inline bool full()
    {
        bool isFull = true;

        if (m_counts == m_size)
            isFull = true;
        else
            isFull = false;

        return isFull;
    }

    /**
     * @brief capacity 该缓冲区的容量大小
     * @return 容量大小
     */
    inline int capacity()
    {
        return m_size;
    }

    /**
     * @brief size 当前已经的有效数据个数
     * @return 有效数据个数
     */
    inline int size()
    {
        int counts = 0;

        mutex.lock();
        counts = m_counts;
        mutex.unlock();

        return counts;
    }

    inline int tail() const
    {
        return m_tail;
    }

private:
    QMutex mutex;

    Type *m_pBuffer;
    int m_size;
    int m_head;
    int m_tail;
    int m_counts;
};

template<typename Type>
RingBuffer<Type>::RingBuffer(int size)
    : m_size(size)
    , m_head(0)
    , m_tail(0)
    , m_counts(0)
{
    if (m_size)
    {
        m_pBuffer = new Type[m_size]();
        if (m_pBuffer == NULL)
        {
            qDebug() << "new RingBufferError!";
            m_size = 0;
        }
    }
}

template<typename Type>
RingBuffer<Type>::~RingBuffer()
{
    m_head = 0;
    m_tail = 0;
    m_counts = 0;
    if (m_pBuffer)
    {
        delete [] m_pBuffer;
        m_pBuffer = NULL;
    }
}

template<typename Type>
int RingBuffer<Type> ::read(Type *pData, int size)
{
    if ((m_pBuffer == NULL) || (pData == NULL) || (size < 0))
        return -1;

    if (empty() || size == 0)
    {
        return 0;
    }

    mutex.lock();

    size = qMin(size, m_counts);

    if (m_tail < m_head)
    {
        memcpy(pData, m_pBuffer + m_tail, size * sizeof(Type));
    }
    else
    {
        if (m_size - m_tail >= size)
            memcpy(pData, m_pBuffer + m_tail, size * sizeof(Type));
        else
        {
            memcpy(pData, m_pBuffer + m_tail, (m_size - m_tail)*sizeof(Type));
            memcpy(pData + m_size - m_tail, m_pBuffer, (size + m_tail - m_size)*sizeof(Type));
        }
    }

    m_tail = (m_tail + size) % m_size;
    m_counts -= size;

    mutex.unlock();

    return size;
}

template<typename Type>
int RingBuffer<Type> ::readOne(Type *pData, int size)
{
    if ((m_pBuffer == NULL) || (pData == NULL) || (size < 0))
        return -1;

    if (empty())// || size == 0)
    {
        return 0;
    }

    mutex.lock();

    size = qMin(size, m_counts);

    if (m_tail < m_head)
    {
        *pData = *(m_pBuffer + m_tail + size);
    }
    else
    {
        if (m_size - m_tail > size)
            *pData = *(m_pBuffer + m_tail + size);
        else
        {
            *pData = *(m_pBuffer + size + m_tail - m_size);
        }
    }

    m_tail = (m_tail + size) % m_size;
    m_counts -= size;

    mutex.unlock();

    return size;
}

template<typename Type>
int RingBuffer<Type> ::peek(Type *pData, int size, int& pos)
{
    if ((m_pBuffer == NULL) || (pData == NULL) || (size < 0))
        return -1;

    if (size == 0)
        return 0;

    if (pos == -1)
    {
        // pos == -1时,读取尽量多的数据
        // 计算出理论的pos位置,如果pos跟head相等,表示没有新的数据进来,此时读取不到数据.
        // 所以将pos向后移动一个索引位置.
        pos = (m_head + m_size - size) % m_size;
        if (pos == m_head)
            pos = (m_head + 1) % m_size;
    }

    if(pos >= m_size)
        pos = 0;

    if (pos == m_head)
    {
        return 0;
    }
    mutex.lock();
    if (m_head > pos)
    {
        //若头在前
        int peekLen = m_head - pos;
        size = qMin(size, peekLen);
        memcpy(pData, m_pBuffer + pos, size * sizeof(Type));
        pos += size;
    }
    else
    {
        int peekLen = m_size - pos + m_head;
        size = qMin(size, peekLen);
        //若是从pos到m_size-1的长度大于等于需要读取的长度
        //就直接拷贝
        if (m_size - pos >= size)
        {
            memcpy(pData, m_pBuffer + pos, size * sizeof(Type));
            pos += size;
        }
        else
        {
            //否则分段拷贝
            int firstSection = m_size - pos;
            memcpy(pData, m_pBuffer + pos, firstSection * sizeof(Type));
            int res = size - firstSection;
            memcpy(pData + firstSection, m_pBuffer, res * sizeof(Type));
            pos = res;
        }
    }

    mutex.unlock();

    return size;
}

template<typename Type>
int RingBuffer<Type>::write(Type *pData, int size)
{
    if ((m_pBuffer == NULL) || (pData == NULL) || (size < 0))
        return -1;

    if (full() || size == 0)
    {
        //printf("RINGBUFFER FULL!\t");
        return 0;
    }
    mutex.lock();

    size = qMin(size, m_size - m_counts);
    if (m_head >= m_tail)
    {
        if ((m_size - m_head) >= size)
        {
            memcpy(m_pBuffer + m_head, pData, size * sizeof(Type));
        }
        else
        {
            memcpy(m_pBuffer + m_head, pData, (m_size - m_head)*sizeof(Type));
            memcpy(m_pBuffer, pData + m_size - m_head, (size + m_head - m_size)*sizeof(Type));
        }
    }
    else
    {
        memcpy(m_pBuffer + m_head, pData, size * sizeof(Type));
    }
    m_head = (m_head + size) % m_size;
    m_counts += size;

    mutex.unlock();

    return size;
}

#endif // RINGBUFFER_H
