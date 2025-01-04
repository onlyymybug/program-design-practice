#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int g_sleep_seconds;

#define TRUE 1
#define FALSE 0
typedef int BOOL;

// �������pattern����λ�õ�next����
// next[n]�ĺ����ǣ�pattern[n]֮ǰ�Ĳ��֣�ǰ׺=��׺����󳤶�
int* build_next(const char* pattern, int length)
{
   int next[256];
   next[0] = -1; // ��ʼ��next[0]Ϊ-1
   int n = 1; // next���α꣬0�Ѿ������ˣ���1��ʼ��
   int i = -1; // pattern���α꣬��ʼֵΪ-1��ָ�򼴽�Ҫƥ���ǰ׺�����һλ
   while(n < length)
   {
      if(i == -1) // ���pattern�α�Ϊ��ʼֵ
      {
         printf("i = -1, so next[%d] = 0\n", n);
         next[n] = 0; // next[n]�϶�Ϊ0����Ϊû��ǰ׺�ͺ�׺ƥ��
         i = 0; // ��һѭ���᳢��ƥ��pattern[0]��pattern[n-1]
         n++; 
      }
      else if(pattern[n - 1] == pattern[i]) // ������ǰ׺=��׺��pattern[n - 1]����׺�����һλ��pattern[i]��ǰ׺�����һλ
      {
         printf("p[%d]:%c == p[%d]:%c, so next[%d] = %d\n", n - 1, pattern[n - 1], i, pattern[i], n, i + 1);
         i++;
         next[n] = i; // ˵������ǰ��ƥ���ǰ׺�ͺ�׺�ֶ���һλ
         n++;
      }
      else // ��ƥ�䣬ǰ�����ǰ׺��׺ƥ�䳤����Ч�ˣ�ֻ���˻�ȥƥ����һ�����ܵ�ǰ׺
      {
         printf("p[%d]:%c != p[%d]:%c, so i(%d) back to next[%d](%d)\n", n - 1, pattern[n - 1], i, pattern[i], i, i, next[i]);
         i = next[i];
      }
   }
   return next;
}

// ��ӡ�����õ�next��
void print_next(const char* pattern, int* next, int length)
{
   printf("\npattern: ");
   for(int i = 0; i <= length; i++)
      printf(" %c ", pattern[i]);
   printf("\n");
   printf("next:    ");
   for(int i = 0; i <= length; i++)
      printf("%2d ", next[i]);
   printf("\n\n");
}

// �����Ǽ�����ӡ��ص�С����֧��
void print_chars(int count, char c)
{
   for(int i = 0; i < count; i++)
      printf("%c", c);
}
void print_spaces(int count)
{
   print_chars(count, ' ');
}
// line_back��line_clear����������windows��ִ�������⣬������㱾��Ҫ��Ѱ�ҵ�BUG
void line_back()
{
#ifndef WIN32
   printf("\033[1A"); //�ص���һ��
#else
   printf("\n");
#endif
}
void line_clear()
{
#ifndef WIN32
   printf("\r");
   printf("\033[K");  //�������
#endif
}

// ��ӡ��ǰ��ƥ��״̬
// ����s������
// ����i�����ݵĵ�ǰָ��
// ����j��pattern�ĵ�ǰָ��
// ����head��ǰ��Ԥ���Ŀհ�
// ����match����ǰ�ַ��Ƿ�ƥ�䣬1ƥ�䣬0��ƥ��
void print_match_state(const char* s, int i, int j, int head, BOOL match)
{
   line_back(); // �ص���һ��
   line_clear(); // �����һ��

   // ��ӡƥ��״̬��
   // �Ѿ�ƥ��ģ���ӡ���ߡ�
   // �µ�ƥ�䣬���ƥ��Ҳ��ӡ���ߣ������ӡx
   print_spaces(head); 
   print_chars(j, '|');
   printf("%c", match ? '|': 'x');

   // ��ӡ��ǰ����
   printf("\n");
   line_clear();
   if(i <= head)
   {
      print_spaces(head - i + j);
      printf("%s", s);
   }
   else
      printf("%s", s + i - j - head);
   fflush(stdout);
}

// ����kmpƥ�䣬��ʾƥ�����
// ����s��Ҫƥ�������
// ����pattern��Ҫƥ���ģʽ
// ����next��ģʽ��next��
// ����pattern_length��ģʽ�ĳ���
const char* kmp(const char* s, const char* pattern, int* next, int pattern_length)
{
   int i = 0; // ����s���α�
   int j = 0; // pattern���α�

#define HEAD_SPACE_COUNT 20
   print_spaces(HEAD_SPACE_COUNT);
   printf("%s\n\n", pattern);
   print_spaces(HEAD_SPACE_COUNT);
   printf("%s", s);
   fflush(stdout);

   while (s[i] != 0 && j < pattern_length)
   {
      sleep(g_sleep_seconds);
      if(s[i] == pattern[j]) // ���ƥ�䣬��ô���ݺ�parrern���α�ͬʱ����1λ
      {
         print_match_state(s, i, j, HEAD_SPACE_COUNT, TRUE);
         i++;
         j++;
      }
      else
      {
         if(j == 0) // ��ƥ�䣬����pattern�α�Ϊ0����һλ�Ͳ�ƥ�䣩����ô�����α�����1λ��pattern�α�ά��0
         {
            print_match_state(s, i, j, HEAD_SPACE_COUNT, FALSE);
            i++;
         }
         else
         {
            print_match_state(s, i, j, HEAD_SPACE_COUNT, FALSE);
            j = next[j]; // ��ƥ�䣬����pattern�α겻��0��pattern����nextָʾ���ˣ������α겻��
         }
      }
   }
   // ��ѭ�����������pattern�α굽��ĩβ����ô��ʾƥ��ɹ�
   if (j >= pattern_length)
      return s + i - pattern_length; // ��������ƥ��pattern����ʼλ��ָ��
   return NULL;
}

// ����1��pattern
// ����2��Ҫƥ�������
// ����3��ÿһ���ȴ���ʱ�䣬��������Ϊ��λ��Ĭ��1�롣
int main(int argc, const char* argv[])
{
   if(argc < 3)
   {
      printf("Usage: %s pattern text [sleep]\n", argv[0]);
      return 1;
   }
   const char* pattern = argv[1];
   const char* text = argv[2];
   g_sleep_seconds = 1;
   if(argc == 4)
      g_sleep_seconds = atoi(argv[3]);

   int pattern_length = strlen(pattern);
   int* next = build_next(pattern, pattern_length);
   print_next(pattern, next, pattern_length);

   const char* p = kmp(text, pattern, next, pattern_length);
   sleep(g_sleep_seconds);
   if(p == NULL)
      printf("\n\nnot match\n\n");
   else
      printf("\n\nmatch at %ld\n\n", p - text);
   return 0;
}
