#include "UI.h"
#include "USER_Hardware.h"
#include "USER_BSP.h"

short UI_chain_LEN = 0;//UI������
short UI_menu_LEN = 0;//��ǰUI�Ĳ˵���������

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
* Description    : ��ʼ��UI
* EntryParameter : None
* ReturnValue    : None
**************************************************************************************/
void UI_Init()
{
    UI_chain_Header = UI_chain;//��ͷָ��
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
* Description    : UI����ִ��
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
    KEY_Value = 0;//�����ֵ
}


/**************************************************************************************
* FunctionName   : UI_ADD_chain(UI_HIGT_TABLE* UI_ADD_chain)
* Description    : �ӳ�UI��ָ����һ����ʾ�Ĳ˵�
* EntryParameter : UI_HIGT_TABLE* UI_ADD_chain
* ReturnValue    : None
**************************************************************************************/

void UI_ADD_chain(UI_HIGT_TABLE* UI_ADD_chain)
{
    ++UI_chain_LEN;//��������
    UI_chain = UI_chain_Header;//��ָ�� ����ص���ͷλ��
    for(int a = 0;a < UI_chain_LEN;a++)
    {
        UI_chain = UI_chain->HIGT_TABLE;
    }
    UI_chain = UI_ADD_chain;
}

/**************************************************************************************
* FunctionName   : UI_BACK_chain()
* Description    : ������һ����
* EntryParameter : None
* ReturnValue    : None
**************************************************************************************/

void UI_BACK_chain()
{
    UI_chain_LEN--;//����������С ���������ʵ�ʳ��Ȳ�û�иı�
    if(UI_chain_LEN < 0)
    {
        UI_chain_LEN = 0;
    }
    UI_chain = UI_chain_Header;//��ָ�� ����ص���ͷλ��
    for(int a = 0;a < UI_chain_LEN;a++)
    {
        UI_chain = UI_chain->HIGT_TABLE;
    }
}

/**************************************************************************************
* FunctionName   : UI_ADD_cursor()
* Description    : ���ֵ���� ��������
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
* Description    : ���ֵ��С ��������
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
* Description    : ˢ��һ����Ļ ��ʾ�˵�
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
