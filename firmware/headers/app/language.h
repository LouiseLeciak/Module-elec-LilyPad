
#ifndef LANGUAGE_H
#define LANGUAGE_H

typedef enum
{
  LANG_FR,
  LANG_EN
} language_t;

void language_switch_init(void);
void language_update(void);
void language_init(void);

#endif
