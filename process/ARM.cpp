#include "ARM.h"
#include "srtf.h"
#include<QDebug>

ARM::ARM(){
    head->front = NULL;
    head->state = "ERROR";
    head->next = last;
    last->front = head;
    last->next = NULL;
    last->id = "NONE";
    last->state = "FREE";
    last->address = 0;
    last->size = 10000;
}


bool ARM::add(PCB &a,int x){
    if(a.printer>x){
        return false;
    }
    int min;
    Arm *flag = new Arm;
    flag->id = a.name;
    flag->size = a.need_arm;
    flag->state = "BUSY";
    Arm *p = head->next;
    Arm *q = NULL;
    while (p != NULL) {
        if (p->state == "FREE" && p->size >= a.need_arm) {
            q = p;
            min = p->size - a.need_arm;
            break;
        }
        p = p->next;
    }
    while (p != NULL) {
        if (p->state == "FREE" && p->size >= a.need_arm) {
            if (p->size - a.need_arm < min) {
                min = p->size - a.need_arm;
                q = p;
            }
        }
        p = p->next;
    }
    if (q == NULL) {
        return false;
    } else if (q->size > a.need_arm) {
        flag->next = q;
        flag->front = q->front;
        flag->address = q->address;
        q->front->next = flag;
        q->front = flag;
        q->size = min;
        q->address += a.need_arm;
    } else {
        q->id = a.name;
        q->size = a.need_arm;
        q->state = "BUSY";
    }
    return true;
}

void ARM::cyc(QString name){
    Arm *p = head->next;
    while (p!=NULL) {
        if(p->id == name){
            p->state = "FREE";
            p->id = "NONE";
            if (p != last) {
                if (p->front->state == "FREE" && p->next->state != "FREE") {
                    p->front->size += p->size;
                    p->front->next = p->next;
                    p->next->front = p->front;
                } else if (p->front->state != "FREE" && p->next->state == "FREE") {
                    p->size += p->next->size;
                    if (p->next->next) {
                        p->next->next->front = p;
                        p->next = p->next->next;
                    } else p->next = p->next->next;
                } else if (p->front->state == "FREE" && p->next->state == "FREE") {
                    p->front->size += p->size + p->next->size;
                    if (p->next->next) {
                        p->next->next->front = p->front;
                        p->front->next = p->next->next;
                    } else p->front->next = p->next->next;
                } else if (p->front->state != "FREE" && p->next->state != "FREE") {
                }
            } else {
                if (p->front->state == "FREE") {
                    p->front->size += p->size;
                    p->front->next = NULL;
                } else if (p->front->state != "FREE") {

                }
            }
        }
        p = p->next;
    }
}

