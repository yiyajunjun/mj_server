#include <stdio.h>
#include <string.h>
#include "wave_table.h"

WaveTable* WaveTable::m_instance = NULL;

WaveTable::WaveTable()
{
    m_collect = false;
    m_key_num = 1 << 18;
    m_keys = new bool[1<<18];
    memset(m_keys, 0, m_key_num/8);
}

WaveTable::~WaveTable()
{
    delete[] m_keys;
}

WaveTable* WaveTable::getInstance()
{
    if(!m_instance)
    {
        m_instance = new WaveTable;
    }
    return m_instance;
}

int WaveTable::getKey(int number)
{
    int bits = 0x0000;
    int shift = 0;
    while(number > 0)
    {
        int n = (number % 10) - 1;
        n = n << shift;
        shift = shift + 2;
        bits = bits | n;
        number = number/10;
    }

    return bits;
}

void WaveTable::setCollect()
{
    m_collect = true;
}
bool WaveTable::check(int number)
{
    int key = getKey(number);

    if(m_collect)
    {
        add(number);
    }

    return m_keys[key];
}

void WaveTable::add(int number)
{
    int key = getKey(number);
    m_keys[key] = true;
}

void WaveTable::dump(char* name)
{
    FILE *fp = fopen(name, "wt+");

    fwrite(m_keys, sizeof(bool), m_key_num, fp);

    fclose(fp);
}

void WaveTable::load(char* name)
{
    FILE *fp = fopen(name, "r");

    fread(m_keys, sizeof(bool), m_key_num, fp);

    fclose(fp);
}