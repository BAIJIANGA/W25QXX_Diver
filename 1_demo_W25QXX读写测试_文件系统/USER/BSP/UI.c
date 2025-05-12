#include "UI.h"
#include "USER_Hardware.h"
#include "USER_BSP.h"

short UI_chain_LEN = 0;//UI链长度
short UI_menu_LEN = 0;//当前UI的菜单功能索引

void UI_NULL(void)
{
    
}

UI_BASE_TABLE First_TABLE[] = 
{
    {0,0,6*4,12,12,BLUE,(uint8_t*)"next",first_fun,ui_lll},
    {0,12,6*6,12,12,BLACK,(uint8_t*)"Second",default_fun_1,UI_NULL},
    {0,24,6*5,12,12,BLACK,(uint8_t*)"Second",default_fun_1,UI_NULL}
};

UI_BASE_TABLE First_TABLE1[] = 
{
    {0,0,6*4,12,12,BLACK,(uint8_t*)"back",ggg_fun,UI_NULL},
    {0,12,6*3,12,12,BLACK,(uint8_t*)"123",default_fun_1,UI_NULL},
    {0,24,6*3,12,12,BLACK,(uint8_t*)"123",default_fun_1,UI_NULL}
};

UI_HIGT_TABLE UI_BASE_chain_2 = {3,First_TABLE1};
UI_HIGT_TABLE UI_BASE_chain_1 = {3,First_TABLE};
UI_HIGT_TABLE* UI_chain = &UI_BASE_chain_1;
UI_HIGT_TABLE* UI_chain_Header;


/**************************************************************************************
* FunctionName   : UI_Init()
* Description    : 初始化UI
* EntryParameter : None
* ReturnValue    : None
**************************************************************************************/
void UI_Init()
{
    UI_chain_Header = UI_chain;//表头指针
    for(int a = 0;a < UI_menu_num;a++)
    {
        LCD_ShowString((UI_chain->BASE_TABLE+a)->x,
                        (UI_chain->BASE_TABLE+a)->y,
                        (UI_chain->BASE_TABLE+a)->menu_name,
                        (UI_chain->BASE_TABLE+a)->color,
                        WHITE,
                        (UI_chain->BASE_TABLE+a)->size,
                        0);
    }
        LCD_ShowString((UI_chain->BASE_TABLE)->x,
                    (UI_chain->BASE_TABLE)->y,
                    (UI_chain->BASE_TABLE)->menu_name,
                    (UI_chain->BASE_TABLE)->color,
                    RED,
                    (UI_chain->BASE_TABLE)->size,
                    0);
}

/**************************************************************************************
* FunctionName   : UI_execute_function()
* Description    : UI功能执行
* EntryParameter : None
* ReturnValue    : None
**************************************************************************************/

void UI_execute_function()
{
    (UI_chain->BASE_TABLE+UI_menu_LEN)->UI_function();
    for(int a = 0;a < UI_menu_num;a++)
    {
        (UI_chain->BASE_TABLE+a)->UI_SHOW_INFOM();
    }
    KEY_Value = 0;//清除键值
}


/**************************************************************************************
* FunctionName   : UI_ADD_chain(UI_HIGT_TABLE* UI_ADD_chain)
* Description    : 加长UI链指向下一级显示的菜单
* EntryParameter : UI_HIGT_TABLE* UI_ADD_chain
* ReturnValue    : None
**************************************************************************************/

void UI_ADD_chain(UI_HIGT_TABLE* UI_ADD_chain)
{
    ++UI_chain_LEN;//链长增加
    UI_chain = UI_chain_Header;//回指针 链表回到表头位置
    for(int a = 0;a < UI_chain_LEN;a++)
    {
        UI_chain = UI_chain->HIGT_TABLE;
    }
    UI_chain = UI_ADD_chain;
}

/**************************************************************************************
* FunctionName   : UI_BACK_chain()
* Description    : 返回上一级链
* EntryParameter : None
* ReturnValue    : None
**************************************************************************************/

void UI_BACK_chain()
{
    UI_chain_LEN--;//索引链长减小 但是链表的实际长度并没有改变
    if(UI_chain_LEN < 0)
    {
        UI_chain_LEN = 0;
    }
    UI_chain = UI_chain_Header;//回指针 链表回到表头位置
    for(int a = 0;a < UI_chain_LEN;a++)
    {
        UI_chain = UI_chain->HIGT_TABLE;
    }
}

/**************************************************************************************
* FunctionName   : UI_ADD_cursor()
* Description    : 光标值增加 框所下移
* EntryParameter : None
* ReturnValue    : None
**************************************************************************************/

void UI_ADD_cursor()
{
    LCD_ShowString(UI_X,UI_Y,UI_name,UI_color,WHITE,UI_size,0);
    if(++UI_menu_LEN >= UI_menu_num)
    {
        UI_menu_LEN = 0;
    }
    LCD_ShowString(UI_X,UI_Y,UI_name,UI_color,RED,UI_size,0);
}

/**************************************************************************************
* FunctionName   : UI_BACK_cursor()
* Description    : 光标值减小 框所上移
* EntryParameter : None
* ReturnValue    : None
**************************************************************************************/

void UI_BACK_cursor()
{
    LCD_ShowString(UI_X,UI_Y,UI_name,UI_color,WHITE,UI_size,0);
    if(--UI_menu_LEN < 0)
    {
        UI_menu_LEN = UI_menu_num-1;
    }
    LCD_ShowString(UI_X,UI_Y,UI_name,UI_color,RED,UI_size,0);
}

/**************************************************************************************
* FunctionName   : UI_Refresh()
* Description    : 刷新一遍屏幕 显示菜单
* EntryParameter : None
* ReturnValue    : None
**************************************************************************************/

void UI_Refresh()
{
    LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
    for(int a = 0;a < UI_menu_num;a++)
    {
        LCD_ShowString((UI_chain->BASE_TABLE+a)->x,
                        (UI_chain->BASE_TABLE+a)->y,
                        (UI_chain->BASE_TABLE+a)->menu_name,
                        (UI_chain->BASE_TABLE+a)->color,
                        WHITE,
                        (UI_chain->BASE_TABLE+a)->size,
                        0);
    }
    LCD_ShowString(UI_X,UI_Y,UI_name,UI_color,RED,UI_size,0);
}
