#include "wioMenuView.hpp"

MenuView::MenuView(){
    menu.menu_itemcnt = 0;
}

/**
 * @param height[in] height
 * @param width[in]  width
*/
void MenuView::init(int width, int height){
    bkview.setColorDepth(8);
    bkview.createSprite(width, 20);
    bkview.fillScreen(TFT_BLACK);
    bkview.clear(TFT_SKYBLUE);

    frview.setColorDepth(8);
    frview.createSprite(width, height);
    bkview.fillScreen(TFT_BLACK);
    frview.setFont(&fonts::Font2);
    frview.setTextColor(TFT_YELLOW);

    keyController.begin();
}

void MenuView::int_cursor_up(void){
    static unsigned long lastCalled = 0;
    if(millis()-lastCalled < 300){
        return;
    }
}
void MenuView::int_cursor_down(void){
    static unsigned long lastCalled = 0;
    if(millis()-lastCalled < 300){
        return;
    }
}

void MenuView::setItem(const char *title ,void (*func)()){
    if(menu.menu_itemcnt >= MAXMENUITEMCNT - 1){
        Serial.printf(" # メニューの登録可能アイテム数の上限に達しました (%s)",title);
        return;
    }
    if(strlen(title) > MAXTITLECHARSIZE){
        strncpy(menu.item[menu.menu_itemcnt].title, title, MAXTITLECHARSIZE);
        menu.item[menu.menu_itemcnt].title[MAXTITLECHARSIZE] = '\0';
    }else{
        strcpy(menu.item[menu.menu_itemcnt].title, title);
    }
    
    menu.item[menu.menu_itemcnt].callfunc = func;
    menu.menu_itemcnt += 1;
}

void MenuView::printItem(uint8_t sidx){
    static uint8_t idx;
    if(sidx != UCHAR_MAX){
        idx = sidx;
    }
    frview.fillScreen(TFT_BLACK);
    for(int i = idx; i < menu.menu_itemcnt; i++){
        frview.setCursor(5,2 + 20 * (i - idx));
        if(i == menu.selector.current_row){ frview.setTextColor(TFT_DARKGREY);}
        else                         { frview.setTextColor(TFT_YELLOW);}

        frview.print(menu.item[i].title);
        //Serial.println(menu.item[i].title);
    }
}

void MenuView::reprintItem(){
    printItem(UCHAR_MAX);
}

void MenuView::begin(){
    printItem(0);
    lastUpdateViewTm = millis();
    
    menu.selector.init();                   /* uintの最大値 */
    if(menu.menu_itemcnt > 0){              /* 選択可能アイテム有*/
        menu.selector.request_row = 0;      /* 1番目を選択 */
    }
}


/**
 * @retval 0x0   : not changed
 * @retval other : number of changes
 */
uint8_t MenuView::update(LGFX_Sprite* view){
    uint8_t ret = 0;
    if(keyController.getKeyOperation(WIO_5S_SHORTUP)){
        //Serial.println("shortUp!!");
        if(menu.selector.current_row > 0){
            menu.selector.request_row = menu.selector.current_row - 1;
            if(menu.yoffset < 0){
                frview_scrolldown();
                ret += 1;
            }
        }
    }else if(keyController.getKeyOperation(WIO_5S_LONGUP)){
        Serial.println("longUp!!");
    }else if(keyController.getKeyOperation(WIO_5S_SHORTDOWN)){
        //Serial.println("shortDown!!");
        if(menu.selector.current_row < menu.menu_itemcnt -1){
            menu.selector.request_row = menu.selector.current_row + 1;
            if(menu.yoffset > ((menu.selector.request_row - 7) * (-20 - 2))){
                frview_scrollup();
                ret += 1;
            }
        }
    }else if(keyController.getKeyOperation(WIO_5S_LONGDOWN)){
        Serial.println("longDown!!"); 
    }else if(keyController.getKeyOperation(WIO_5S_SHORTPRESS)){
        Serial.println("shortPress!!");
        if(menu.item[menu.selector.current_row].callfunc != NULL)
        {
            (*menu.item[menu.selector.current_row].callfunc)();
        }
    }else if(keyController.getKeyOperation(WIO_5S_LONGPRESS)){
        Serial.println("longPress!!"); 
    }else{
    }

    if(menu.selector.current_row != menu.selector.request_row)
    {
        uint16_t pitem = menu.selector.current_row;/* 変更前 */
        uint16_t nitem = menu.selector.request_row;/* 変更後 */
        
        //Serial.printf("セレクタ変更検出 [%d][%d]\n", pitem, nitem);

        /* セレクタ動作演出 */
        int diff = (2 + 20 * nitem) - menu.selector.current_loc;

        if(diff > 0){
            if(diff <= SELECTOR_MOVESPD){
                menu.selector.current_loc += diff;
                /*
                frview.setTextColor(TFT_DARKGREY); 
                frview.setCursor(5,2 + 20 * nitem);
                frview.print(menu.item[nitem].title);
                */
                menu.selector.current_row = nitem;
                reprintItem();
                ret += 1;
            }else{
                menu.selector.current_loc += SELECTOR_MOVESPD;
            }

            /* 元の選択位置の選択解除演出 */
            int diff = menu.selector.current_loc - (2 + 20 * pitem);
            if(diff > 10){
                /*
                frview.setTextColor(TFT_YELLOW);
                frview.setCursor(5,2 + 20 * pitem); //演出終了
                frview.print(menu.item[pitem].title);
                */
                
            }
        }else if(diff < 0){
            diff *= -1;                             /* 符号反転 */
            if(diff <= SELECTOR_MOVESPD){
                menu.selector.current_loc -= diff;
                /*
                frview.setTextColor(TFT_DARKGREY);
                frview.setCursor(5,2 + 20 * nitem);
                frview.print(menu.item[nitem].title);
                */
                menu.selector.current_row = nitem;
                reprintItem();
                ret += 1;
            }else{
                menu.selector.current_loc -= SELECTOR_MOVESPD;
            }

            /* 元の選択位置の選択解除演出 */
            int diff = (2 + 20 * pitem) - menu.selector.current_loc;
            if(diff > 10){
                /*  
                frview.setTextColor(TFT_YELLOW);
                frview.setCursor(5,2 + 20 * pitem);  // 演出終了
                frview.print(menu.item[pitem].title);
                */
            }
        }else if(pitem != nitem){   /* menu初期化時等に実行される */
                frview.setTextColor(TFT_DARKGREY);
                frview.setCursor(5,2 + 20 * nitem);  // 演出終了
                frview.print(menu.item[nitem].title);
                menu.selector.current_row = nitem;
        }
    }

    frview.pushSprite(view, 0,  0, TFT_BLACK);
    return ret;
}

void MenuView::frview_scrollup(){
    if(menu.menu_itemcnt < MAXVISIBLEITEMCNT)
    {
        return;
    }
    menu.yoffset -= 20 + 2;
    printItem(menu.yoffset / -(20+2));
}
void MenuView::frview_scrolldown(){
    menu.yoffset += 20 + 2;
    printItem(menu.yoffset / -(20+2));
}