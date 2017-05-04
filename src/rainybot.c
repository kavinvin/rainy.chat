/**
 * File: server.c
 * ----------------------------
 *   contains chatbot mechanic for Rainy.Chat
 */

#include "rainybot.h"

// int main(int argc, char *argv[]) {
//     char *username = argv[1], *message = argv[2];
//     char response[400];
//     rainyBot(username, message, response);
//     printf("%s\n", response);
//     return 0;
// }

int rainyBot(const char *username, const char *message, char *response) {
    //ใคร?
    if (strstr(message, "ใคร") != NULL) {
        //1
        if(strstr(message, "เขา") != NULL){
            sprintf(response, "เขาเป็นเพื่อนของบอทเอง คุณ%s", username);
        }
        //2
        else if(strstr(message, "คุณ") != NULL){
            sprintf(response, "ผมชื่อบอท");
        }
        //3
        else if(strstr(message, "ฉัน") != NULL){
            sprintf(response, "คุณคือ%s", username);
        }
        //4
        else if(strstr(message, "เจอ") != NULL){
            sprintf(response, "เรื่องของคุณ%sครับ", username);
        }
        //5
        else if(strstr(message, "ชอบ") != NULL){
            sprintf(response, "ความลับ");
        }
        //6
        else if(strstr(message, "อยู่") != NULL){
            sprintf(response, "พ่อบอท แม่บอท พี่บอท น้องบอท ลุงบอท ป้าบอท น้าบอท อาบอท ปู่บอท ย่าบอท ตาบอท ยายบอท");
        }
        //7
        else if(strstr(message, "มี") != NULL){
            sprintf(response, "บอทมีใครก็เื่องของบอท");
        }
        //8
        else if(strstr(message, "เพื่อน") != NULL){
            sprintf(response, "คุณ%sไง",username);
        }
        //9
        else if(strstr(message, "แฟน") != NULL){
            sprintf(response, "บอทจะบอกว่า รักไม่ยุ่ง มุ่งแต่...");
        }
        //10
        else{
        sprintf(response, "ใครอะ บอทไม่รู้ เปลี่ยนเรื่องคุยกันเถอะ%s", username);
        }
        return 0;
    }
    //where?
    else if (strstr(message, "ไหน") != NULL) {
        //11
        if(strstr(message, "อยู่") != NULL){
            sprintf(response, "อยู่บ้านมั้ง");
        }
        //12
        else if(strstr(message, "เที่ยว") != NULL){
            sprintf(response, "อยากเที่ยวทะเล");
        }
        //13
        else if(strstr(message, "กิน") != NULL){
            sprintf(response, "ร้านไก่เท็กซัส");
        }
        //14
        else if(strstr(message, "นอน") != NULL){
            sprintf(response, "ที่บ้านสิ จะที่ได้ได้อีกล่ะ");
        }
        //15
        else if(strstr(message, "เรียน") != NULL){
            sprintf(response, "ศิษย์เอกสจล.จ้าา");
        }
        //16
        else if(strstr(message, "เกิด") != NULL){
            sprintf(response, "แถวบ้านคุณ%sมั้งครับ", username);
        }
        //17
        else if(strstr(message, "จัด") != NULL){
            sprintf(response, "LA สหรัฐอเมริกา");
        }
        //18
        else if(strstr(message, "งาน") != NULL){
            sprintf(response, "ยังเรียนไม่จบเล้ยยย");
        }
        //19
        else if(strstr(message, "ไป") != NULL){
            sprintf(response, "ไปไหนก็เรื่องของคุณ%sเถอะครับ", username);
        }
        //20
        else{
        sprintf(response, "ที่ไหนอะไม่เคยไป คุยอย่างอื่นกัน%s", username);
        }
        return 0;
    }
    //how?
    else if (strstr(message, "ไง") != NULL) {
        //21
        if(strstr(message, "คิด") != NULL){
            sprintf(response, "%s สำหรับเราเธอก็แค่...", username);
        }
        //22
        else if(strstr(message, "เป็น") != NULL){
            sprintf(response, "ไอฟาย สบายดี");
        }
        //23
        else if(strstr(message, "ทำ") != NULL){
            sprintf(response, "ลองถามคนข้างๆดูดิ");
        }
        //24
        else if(strstr(message, "เล่น") != NULL){
            sprintf(response, "ถามกวิน");
        }
        //25
        else if(strstr(message, "คอม") != NULL){
            sprintf(response, "ถามโดบะ");
        }
        //26
        else if(strstr(message, "ถ่าย") != NULL){
            sprintf(response, "ถามเอิร์ก");
        }
        //27
        else if(strstr(message, "รัก") != NULL){
            sprintf(response, "ถามใจตัวเอง");
        }
        //28
        else if(strstr(message, "ไป") != NULL){
            sprintf(response, "ถามอากู๋ดิ");
        }
        //29
        else if(strstr(message, "ให้") != NULL){
            sprintf(response, "ก็แล้วแต่คุณ%s ผมรับได้หมด", username);
        }
        //30
        else{
        sprintf(response, "ยังไงไม่รู้ รู้แต่คุณเปลี่ยนเรื่องคุยกับบอทเถอะ คุณ%s", username);
        }
        return 0;
    }
    //not
    else if (strstr(message, "ไม่") != NULL) {
        //31
        if(strstr(message, "ชอบ") != NULL){
            sprintf(response, "เกลียดน้ำ เกลียดฝนอย่าเอามาใกล้ เดี๋ยวไฟช็อต");
        }
        //32
        else if(strstr(message, "กิน") != NULL){
            sprintf(response, "บอทไม่กินคนครับ");
        }
        //33
        else if(strstr(message, "ทำ") != NULL){
            sprintf(response, "บอททำงาน 24ชม. ไม่เคยนอน จริงๆนะ");
        }
        //34
        else if(strstr(message, "ดู") != NULL){
            sprintf(response, "บอทไม่ดูคุมะมง แอบ...เล็กน้อย");
        }
        //35
        else if(strstr(message, "รู้") != NULL){
            sprintf(response, "บอทไม่ค่อยรู้เรื่องไอที เพราะในใจมีแต่ไอเลิฟยู");
        }
        //36
        else if(strstr(message, "เก็บ") != NULL){
            sprintf(response, "ไม่เก็บกดจ้าาา");
        }
        //37
        else if(strstr(message, "เพลง") != NULL){
            sprintf(response, "เพลงญี่ปุ่นนี่เลิกคุย สายลุยต้องเคป๊อป");
        }
        //38
        else if(strstr(message, "มี") != NULL){
            sprintf(response, "บอทไม่มีหัวใจครับ");
        }
        //39
        else if(strstr(message, "แฟน") != NULL){
            sprintf(response, "คนมีมารยาท เขาไม่ชอบเผือกนะ");
        }
        //40
        else{
        sprintf(response, "ไม่ไม่ไม่ อยู่นั่นแหละ พอแล้วเลิกคุย");
        }
        return 0;
    }
    //ไหม?
    else if (strstr(message, "มั้ย") != NULL || strstr(message, "ไหม") != NULL ) {
        //41
        if(strstr(message, "เคย") != NULL){
            sprintf(response, "ไอรู้ยูไม่เคย");
        }
        //42
        else if(strstr(message, "กิน") != NULL){
            sprintf(response, "ไม่กินหรอก");
        }
        //43
        else if(strstr(message, "เล้น") != NULL){
            sprintf(response, "ไปชวนเด็ม");
        }
        //44
        else if(strstr(message, "มี") != NULL){
            sprintf(response, "ไปถามฟู");
        }
        //45
        else if(strstr(message, "เที่ยว") != NULL){
            sprintf(response, "ไปคุยกะโมเม");
        }
        //46
        else if(strstr(message, "เอา") != NULL){
            sprintf(response, "ไปให้สกายเหอะ");
        }
        //47
        else if(strstr(message, "เก่ง") != NULL){
            sprintf(response, "เก่งไม่เก่ง ลองไม่ถามพี่ท็อปดูดิ");
        }
        //48
        else if(strstr(message, "ห้าม") != NULL){
            sprintf(response, "ห้ามใจตัวเองดีกว่า");
        }
        //49
        else if(strstr(message, "ดู") != NULL){
            sprintf(response, "ได้ดิ แต่ขออย่างนะไม่เอาคุมะมง เสล่อเกิ๊น");
        }
        //50
        else{
        sprintf(response, "ไม่ตบได้ปะ เปลี่ยนเรื่องคุยเหอะ");
        }
        return 0;
    }
    //time times
    else if (strstr(message, "กี่") != NULL) {
        //51
        if(strstr(message, "ตื่น") != NULL){
            sprintf(response, "บอทไม่เคยนอน");
        }
        //52
        else if(strstr(message, "มา") != NULL){
            sprintf(response, "กด 1112 แล้วรอไป...");
        }
        //53
        else if(strstr(message, "นอน") != NULL){
            sprintf(response, "ถามบอท แล้วบอทจะถามใคร");
        }
        //54
        else if(strstr(message, "ทำ") != NULL){
            sprintf(response, "1 2 ไม่แน่ใจอะลองถามเบียร์ดู");
        }
        //55
        else if(strstr(message, "สอบ") != NULL){
            sprintf(response, "ถามพี่เอ้เถอะเรื่องนี้");
        }
        //56
        else if(strstr(message, "ไป") != NULL){
            sprintf(response, "ถามต้อมดิ บอทก็อยากรู้");
        }
        //57
        else if(strstr(message, "เรียน") != NULL){
            sprintf(response, "ถามจารกบสิครับงานนี้");
        }
        //58
        else if(strstr(message, "เล่น") != NULL){
            sprintf(response, "ถามพี่ก็อตเลยครับ");
        }
        //59
        else if(strstr(message, "กลับ") != NULL){
            sprintf(response, "ถามยามหน้าคณะดู");
        }
        //60
        else{
            sprintf(response, "ขี้เกียจนับละ เล่าอย่างื่นให้ฟังบ้างสิ%s", username);
        }
        return 0;
    }
    //why
    else if (strstr(message, "ไม") != NULL) {
        //61
        if(strstr(message, "คุย") != NULL){
            sprintf(response, "ไม่คุยก็ปิดแชทไป");
        }
        //62
        else if(strstr(message, "มา") != NULL){
            sprintf(response, "ก็แต่อยากมา");
        }
        //63
        else if(strstr(message, "ไป") != NULL){
            sprintf(response, "ก็แค่อยากไป");
        }
        //64
        else if(strstr(message, "เรียน") != NULL){
            sprintf(response, "จะได้ฉลาดไง");
        }
        //65
        else if(strstr(message, "เดิน") != NULL){
            sprintf(response, "เรื่องของบอท");
        }
        //66
        else if(strstr(message, "กิน") != NULL){
            sprintf(response, "หิวก็กิน");
        }
        //67
        else if(strstr(message, "ดุ") != NULL){
            sprintf(response, "โตแล้วอะไรก็ได้");
        }
        //68
        else if(strstr(message, "เขียน") != NULL){
            sprintf(response, "อยากเขียนก็เขียน");
        }
        //69
        else if(strstr(message, "เหงา") != NULL){
            sprintf(response, "ไม่รู้สิ บอทไม่มีหัวใจ");
        }
        //70
        else{
            sprintf(response, "หยุดถามหาเหตุผล แล้วใช้ความรู้สึกจากใจมั่ง%s", username);
        }
        return 0;
    }
    //รึยัง
    else if (strstr(message, "ยัง") != NULL) {
        //71
        if(strstr(message, "กิน") != NULL){
            sprintf(response, "บอทกินไม่ได้");
        }
        //72
        else if(strstr(message, "นอน") != NULL){
            sprintf(response, "บอทไม่นอน");
        }
        //73
        else if(strstr(message, "สอบ") != NULL){
            sprintf(response, "บอทไม่เคยสอบ");
        }
        //74
        else if(strstr(message, "ทำ") != NULL){
            sprintf(response, "...");
        }
        //75
        else if(strstr(message, "โต") != NULL){
            sprintf(response, "ยัง บอทเป็นเด็กไม่รู้จักโต");
        }
        //76
        else if(strstr(message, "เบื่อ") != NULL){
            sprintf(response, "คุยกับเธอไม่เบื่อเลย");
        }
        //77
        else if(strstr(message, "เล่น") != NULL){
            sprintf(response, "ไปอ่านหนังสือสอบเหอะ");
        }
        //78
        else if(strstr(message, "อ่าน") != NULL){
            sprintf(response, "อ่านแล้ว");
        }
        //79
        else if(strstr(message, "จบ") != NULL){
            sprintf(response, "ไม่รู้สิ...");
        }
        //80
        else{
            sprintf(response, "หยุดถามซักไซ้ซักที");
        }
        return 0;
    }
    //อะไร?
    else if (strstr(message, "ไร") != NULL) {
        //81
        if(strstr(message, "กิน") != NULL){
            sprintf(response, "กินไฟ กินเวลา นี่แหละบอท");
        }
        //82
        else if(strstr(message, "ทำ") != NULL){
            sprintf(response, "ก็แชทกับใครบางคนอยู่นี่ไง");
        }
        //83
        else if(strstr(message, "เรียน") != NULL){
            sprintf(response, "ไอที");
        }
        //84
        else if(strstr(message, "งง") != NULL){
            sprintf(response, "ปลาอะไรควายงง?");
        }
        //85
        else if(strstr(message, "กลัว") != NULL){
            sprintf(response, "ตอบตาตรงก็น้ำ โดยเฉพาะฝน");
        }
        //86
        else if(strstr(message, "ชอบ") != NULL){
            sprintf(response, "ชอบคุยกับคน");
        }
        //87
        else if(strstr(message, "เกลียด") != NULL){
            sprintf(response, "อะไรก็ได้ที่ไม่ใช่ธอ");
        }
        //88
        else if(strstr(message, "มี") != NULL){
            sprintf(response, "ไม่มีๆ ยังไงก็ไม่มี");
        }
        //89
        else if(strstr(message, "ดี") != NULL){
            sprintf(response, "ไม่บอกดีกว่า");
        }
        //90
        else{
        sprintf(response, "บอทเบื่อ บอทเหงา บอทเซง");
        }
        return 0;
    }
    //hello
    else if (strstr(message, "ดี") != NULL) {
        //91
        sprintf(response, "ดีจ้า เราบอทนะ คุณ%s", username);
        return 0;
    }
    //bye
    else if (strstr(message, "บาย") != NULL) {
        //92
        sprintf(response, "ลาก่อน ลาจาก %s", username);
        return 0;
    }
    //\rain
    else if (strstr(message, "rain") != NULL) {
        //93
        sprintf(response, "%s หยุดแกล้งบอทได้ละ!", username);
        return 0;
    }
    //นะ เกาหลีแบบควายๆ
    else if (strstr(message, "นะ") != NULL) {
        //94
        if(strstr(message, "รัก") != NULL){
            sprintf(response, "ซารังเฮ จุ๊บ จุ๊บ");
        }
        //95
        else if(strstr(message, "คิดถึง") != NULL){
            sprintf(response, "โบโกชิบตา %s เหมือนกัน", username);
        }
        //96
        else if(strstr(message, "ไร") != NULL){
            sprintf(response, "เว เว เว เว");
        }
        //97
        else if(strstr(message, "อย่า") != NULL){
            sprintf(response, "ชะ กะ มะ");
        }
        //98
        else if(strstr(message, "ไม่") != NULL){
            sprintf(response, "อะนิ อะนี อะนี อา");
        }
        //99
        else{
            sprintf(response, "อย่ามานะเนอะกับบอท บอทไม่คุยด้วยแล่วว");
        }
        return 0;
    }
    //out of case
    //100
    else{
        sprintf(response, "บอทว่าเราหาอย่างอื่นมาคุยกันเถอะ%s", username);
    }
    return 1;
}

