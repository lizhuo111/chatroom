#pragma once
#include"cassert"
//���ͺ궨��
typedef unsigned char     uint8;
typedef unsigned long    uint32;

//��������ֵ�
static uint8 alphabet_map[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
//��������ֵ�
static uint8 reverse_map[] =
{
        255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 62, 255, 255, 255, 63,
        52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 255, 255, 255, 255, 255, 255,
        255,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
        15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 255, 255, 255, 255, 255,
        255, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
        41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 255, 255, 255, 255, 255
};

/*
*  ����
*     ������Ҫ��������ݵ�ַ�����ݳ���
*  ����:����������
*/
uint8* base64_encode(const uint8* text, uint32 text_len)
{
    //������������ݳ���
    //�����Ͽ�֪  Base64���ǽ�3���ֽڵ�����(24λ)�����4��6λ�����ݣ�Ȼ��ǰ��λ����
    //����ת��Ϊ0-63������  Ȼ����ݱ����ֵ���б���
    int encode_length = text_len / 3 * 4;
    if (text_len % 3 > 0)
    {
        encode_length += 4;
    }

    //Ϊ��������ݴ�ŵ�ַ�����ڴ�
    uint8* encode = (uint8*)malloc(encode_length);

    //����
    uint32 i, j;
    for (i = 0, j = 0; i + 3 <= text_len; i += 3)
    {
        encode[j++] = alphabet_map[text[i] >> 2];                             //ȡ����һ���ַ���ǰ6λ���ҳ���Ӧ�Ľ���ַ�
        encode[j++] = alphabet_map[((text[i] << 4) & 0x30) | (text[i + 1] >> 4)];     //����һ���ַ��ĺ�2λ��ڶ����ַ���ǰ4λ������ϲ��ҵ���Ӧ�Ľ���ַ�
        encode[j++] = alphabet_map[((text[i + 1] << 2) & 0x3c) | (text[i + 2] >> 6)];   //���ڶ����ַ��ĺ�4λ��������ַ���ǰ2λ��ϲ��ҳ���Ӧ�Ľ���ַ�
        encode[j++] = alphabet_map[text[i + 2] & 0x3f];                         //ȡ���������ַ��ĺ�6λ���ҳ�����ַ�
    }

    //������󲻹�3���ֽڵ�  �������
    if (i < text_len)
    {
        uint32 tail = text_len - i;
        if (tail == 1)
        {
            encode[j++] = alphabet_map[text[i] >> 2];
            encode[j++] = alphabet_map[(text[i] << 4) & 0x30];
            encode[j++] = '=';
            encode[j++] = '=';
        }
        else //tail==2
        {
            encode[j++] = alphabet_map[text[i] >> 2];
            encode[j++] = alphabet_map[((text[i] << 4) & 0x30) | (text[i + 1] >> 4)];
            encode[j++] = alphabet_map[(text[i + 1] << 2) & 0x3c];
            encode[j++] = '=';
        }
    }
    return encode;
}



uint8* base64_decode(const uint8* code, uint32 code_len)
{
    //�ɱ��봦��֪��������base64����һ����4�ı������ֽ�
    assert((code_len & 0x03) == 0);  //��������������ش�������ֹ����ִ�С�4�ı�����

    //Ϊ���������ݵ�ַ�����ڴ�
    uint8* plain = (uint8*)malloc(code_len / 4 * 3);

    //��ʼ����
    uint32 i, j = 0;
    uint8 quad[4];
    for (i = 0; i < code_len; i += 4)
    {
        for (uint32 k = 0; k < 4; k++)
        {
            quad[k] = reverse_map[code[i + k]];//���飬ÿ���ĸ��ֱ�����ת��Ϊbase64���ڵ�ʮ������
        }

        assert(quad[0] < 64 && quad[1] < 64);

        plain[j++] = (quad[0] << 2) | (quad[1] >> 4); //ȡ����һ���ַ���Ӧbase64���ʮ��������ǰ6λ��ڶ����ַ���Ӧbase64���ʮ��������ǰ2λ�������

        if (quad[2] >= 64)
            break;
        else if (quad[3] >= 64)
        {
            plain[j++] = (quad[1] << 4) | (quad[2] >> 2); //ȡ���ڶ����ַ���Ӧbase64���ʮ�������ĺ�4λ��������ַ���Ӧbase64���ʮ��������ǰ4λ�������
            break;
        }
        else
        {
            plain[j++] = (quad[1] << 4) | (quad[2] >> 2);
            plain[j++] = (quad[2] << 6) | quad[3];//ȡ���������ַ���Ӧbase64���ʮ�������ĺ�2λ���4���ַ��������
        }
    }
    return plain;
}
