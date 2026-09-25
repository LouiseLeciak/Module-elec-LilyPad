#include "utils/mem_utils.h"

void* ft_memcpy(void* restrict dst, const void* restrict src, size_t n)
{
  size_t i = 0;
  char* dup_src = (char*)src;
  char* dup_dst = (char*)dst;

  if (!src && !dst)
  {
    return (dst);
  }

  while (i < n)
  {
    dup_dst[i] = dup_src[i];
    i++;
  }

  return (dst);
}
