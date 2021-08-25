#include <Arduino.h>
#include <LovyanGFX.hpp>
#include <functional>
#include "wioKeyOperation.hpp"

#define MAXTITLECHARSIZE 40
#define MAXMENUITEMCNT 10
#define MAXVISIBLEITEMCNT 10
#define SELECTOR_MOVESPD 15

struct menuItem{
    char title[MAXTITLECHARSIZE + 1] ={0};
    void (*callfunc)() = NULL;
    int menu_itemcnt;
};

struct menuSelector{
    uint16_t current_row;
    uint16_t request_row;

    int32_t  current_loc;  /* 演出用 */

    void init(){
        current_loc = 2 + 20 * 0;
        current_row = -1;
        request_row = 0;
    }
};

struct menuTree{
    menuItem *item;
    int menu_itemcnt;
    menuSelector selector;

    int yoffset = 0;      /* kari */

    menuTree()
    {
        item = (menuItem *)malloc(sizeof(menuItem) * MAXMENUITEMCNT);
    }

    menuTree(size_t maxitmcnt)
    {
        item = (menuItem *)malloc(sizeof(menuItem) * maxitmcnt);
    }
};



static KeyOperation keyController;
class MenuView{

    private:
        unsigned long lastUpdateViewTm;
        LGFX_Sprite frview;
        LGFX_Sprite bkview;
        menuTree menu;
        int view_width;
        int view_height;
        

        void int_cursor_up(void);
        void int_cursor_down(void);
        void int_cursor_enter(void);

        void frview_scrollup();
        void frview_scrolldown();

        void printItem(uint8_t);
        void reprintItem();

        uint8_t getCursor(void);


    public:
        MenuView(void);

        void init(int, int);

        void begin();
        void end();

        uint8_t update(LGFX_Sprite*);

        void setItem(const char *title ,void (*func)());

        uint16_t getSelectedIdx(){ return menu.selector.current_row; };

        menuTree getMenuTree()
        {
            Serial.println(menu.menu_itemcnt);
            return menu;
        }
        void     setMenuTree(menuTree subst)
        {
            menu = subst;
            Serial.println(menu.menu_itemcnt);
            printItem(menu.yoffset / -(20+2));
        }
};

