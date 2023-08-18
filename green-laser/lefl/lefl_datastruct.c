/*
 * lefl_datastruct.c
 */
#include "lefl.h"


lefl_array_elm_t lefl_loop_array_get(lefl_loop_array_t *arr,int16_t j)
{
    if(j>=0&&j<arr->len)
    {
        return arr->list[arr->index-j>=0?arr->index-j:arr->index-j+arr->len];
    }
    else
    {
        return 0;
    }
}

void lefl_loop_array_push_back(lefl_loop_array_t *arr,lefl_array_elm_t t)
{
    arr->index++;
    if(arr->index>=arr->len)
    {
        arr->index=0;
    }
    arr->list[arr->index]=t;
}

lefl_array_elm_t lefl_loop_array_max(lefl_loop_array_t *arr)
{
    lefl_array_elm_t max = 0;
    for(uint8_t i=0;i<arr->len;i++)
    {
        if(arr->list[i]>max)
            max=arr->list[i];
    }
    return max;
}

void lefl_bit_array_set(lefl_bit_array_t* arr, int16_t n,bool b)
{
    if(n>=0&&n<arr->len)
    {
        arr->list[n/LEFL_BIT_ARRAY_UNIT_WIDTH]&=
            (~(1<<(n%LEFL_BIT_ARRAY_UNIT_WIDTH)));
        arr->list[n/LEFL_BIT_ARRAY_UNIT_WIDTH]|=
            (b<<(n%LEFL_BIT_ARRAY_UNIT_WIDTH));
    }
}

bool lefl_bit_array_get(lefl_bit_array_t* arr, int16_t n)
{
    if(n>=0&&n<arr->len)
        return 1&
            (arr->list[n/LEFL_BIT_ARRAY_UNIT_WIDTH]>>(n%LEFL_BIT_ARRAY_UNIT_WIDTH));
    else
        return false;
}

void lefl_bit_array_shift(lefl_bit_array_t* arr, int16_t n)
{
    if(n>0)
    {
        for(int16_t i=(arr->len-1)/LEFL_BIT_ARRAY_UNIT_WIDTH;i>0;i--)
        {
            arr->list[i]<<=n;
            arr->list[i]|=(arr->list[i-1]>>(LEFL_BIT_ARRAY_UNIT_WIDTH-n));
        }
        arr->list[0]<<=n;
    }
    if(n<0)
    {
        for(int16_t i=0;i<(arr->len-1)/LEFL_BIT_ARRAY_UNIT_WIDTH;i++)
        {
            arr->list[i]>>=(-n);
            arr->list[i]|=arr->list[i+1]<<(LEFL_BIT_ARRAY_UNIT_WIDTH-(-n));
        }
        arr->list[(arr->len-1)/LEFL_BIT_ARRAY_UNIT_WIDTH]>>=(-n);
    }
}


lefl_stack_elm_t lefl_stack_get(lefl_stack_t* stack, lefl_stack_elm_t j)
{
    if(j>0 && j < stack->top)
        return stack->list[j];
    else
        return 0;
}
void lefl_stack_push(lefl_stack_t* stack, lefl_stack_elm_t t)
{
    if(stack->top+1<stack->len)
    {
        stack->list[stack->top]=t;
        stack->top++;
    }
}
lefl_stack_elm_t lefl_stack_pop(lefl_stack_t* stack, lefl_stack_elm_t *t)
{
    if(stack->top>0)
    {
        stack->top--;
        *t = stack->list[stack->top];
    }
    return *t;
}
