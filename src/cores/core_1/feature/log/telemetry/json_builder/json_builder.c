#include "json_builder.h"
#include "../../../../../core_utility/core_utility.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Json_t{
  char* p_str_json;
  uint32_t str_json_size;
  uint32_t json_cursor_offset;
};

union Json_h_t_conv {
  Json_h* const hidden;
  struct Json_t* const clear;
};

union Json_h_t_conv_const {
  const Json_h* const hidden;
  const struct Json_t* const clear;
};

#ifdef DEBUG
char __assert_size_json[(sizeof(Json_h)==sizeof(struct Json_t))?1:-1];
char __assert_size_json[(_Alignof(Json_h)==_Alignof(struct Json_t))?1:-1];
#endif /* ifdef DEBUG */

//private

//public

int8_t json_init(Json_h* const restrict self)
{
  union Json_h_t_conv conv = {self};
  struct Json_t* const p_self = conv.clear;

  const char initial_json[] = "{ }";

  memset(p_self, 0, sizeof(*p_self));

  p_self->str_json_size = 3;
  p_self->p_str_json = calloc(1,p_self->str_json_size + 1);
  p_self->json_cursor_offset = 1;
  memcpy(p_self->p_str_json, initial_json, p_self->str_json_size);

  return 0;
}

int8_t json_push_element(Json_h* const restrict self,
    const char* const restrict var_name, const uint8_t size, DataFormat const data_format,
    const void* value)
{
  union Json_h_t_conv conv = {self};
  struct Json_t* const p_self = conv.clear;
  char new_json_field[256] = {0};
  uint8_t new_json_cursor = 0;
  uint32_t var_name_length = strlen(var_name);

  const float* const pf=value;
  uint32_t unsigned_signed_number=0;

  if (p_self->json_cursor_offset > 1)
  {
    new_json_field[new_json_cursor] = ',';
    new_json_cursor+=1;
  }

  new_json_field[new_json_cursor] = '"';
  new_json_cursor+=1;

  memcpy(&new_json_field[new_json_cursor], var_name, var_name_length);
  new_json_cursor+=var_name_length;


  new_json_field[new_json_cursor] = '"';
  new_json_cursor+=1;

  new_json_field[new_json_cursor] = ':';
  new_json_cursor+=1;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat"
  switch (data_format)
  {
    case SIGNED:
      memcpy(&unsigned_signed_number, value, size);
      new_json_cursor += snprintf(&new_json_field[new_json_cursor],32 ,"%ld", unsigned_signed_number);
      break;
    case UNSIGNED:
      memcpy(&unsigned_signed_number, value, size);
      new_json_cursor += snprintf(&new_json_field[new_json_cursor],32 ,"%lu", unsigned_signed_number);
      break;
    case FLOAT:
      new_json_cursor += snprintf(&new_json_field[new_json_cursor],32 ,"%.2f", *pf);
      break;
      break;
    case STRING:
      new_json_field[new_json_cursor] = '"';
      new_json_cursor+=1;

      new_json_cursor += snprintf(&new_json_field[new_json_cursor],32 ,"%s", (char*) value);

      new_json_field[new_json_cursor] = '"';
      new_json_cursor+=1;
      break;
    }
#pragma GCC diagnostic pop

  new_json_field[new_json_cursor] = '}';
  new_json_cursor+=1;

  new_json_field[new_json_cursor] = '\0';
  new_json_cursor+=1;

  p_self->str_json_size += (new_json_cursor - 2);
  p_self->p_str_json = realloc(p_self->p_str_json, p_self->str_json_size + 1);
  if (!p_self->p_str_json)
  {
    SET_TRACE(CORE_1, OUT_OF_HEAP);
    return -99;
  }

  memcpy(&p_self->p_str_json[p_self->json_cursor_offset], new_json_field, new_json_cursor);
  p_self->json_cursor_offset += (new_json_cursor - 2);

  return 0;
}

const char* json_get(const Json_h* const restrict self)
{
  const union Json_h_t_conv_const conv = {self};
  const struct Json_t* const p_self = conv.clear;

  return p_self->p_str_json;
}

uint32_t json_len(const Json_h* const restrict self)
{
  const union Json_h_t_conv_const conv = {self};
  const struct Json_t* const p_self = conv.clear;

  return strlen(p_self->p_str_json);
}

int8_t json_destroy(Json_h* self)
{
  union Json_h_t_conv conv = {self};
  struct Json_t* const p_self = conv.clear;

  free(p_self->p_str_json);
  memset(p_self, 0, sizeof(*p_self));

  return 0;
}

void
json_print(Json_h* self)
{
  union Json_h_t_conv conv = {self};
  struct Json_t* const p_self = conv.clear;

  printf("%s\n", p_self->p_str_json);
}
