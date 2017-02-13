#include "bmobjsonutil.h"
#include "../cocos/base/ccMacros.h"


void BmobJsonUtil::object2Json(Json::Value *value, std::string key, Ref *object){
    if (!object) {
        return;
    }
    if (dynamic_cast<__Integer *>(object)) {
        (*value)[key] = dynamic_cast<__Integer *>(object)->getValue();
    }else if (dynamic_cast<__String *>(object)) {
        (*value)[key] = dynamic_cast<__String *>(object)->getCString();
    }else if (dynamic_cast<__Float *>(object)) {
        (*value)[key] = dynamic_cast<__Float *>(object)->getValue();
    }else if (dynamic_cast<__Double *>(object)) {
        (*value)[key] = dynamic_cast<__Double *>(object)->getValue();
    }else if (dynamic_cast<__Bool *>(object)) {
        (*value)[key] = dynamic_cast<__Bool *>(object)->getValue();
    }else if(dynamic_cast<__Dictionary*>(object)){
        Json::Value data;
        //string key1 = "__op";
         //返回一个包含所有key的__Array
       __Dictionary* dict = dynamic_cast<__Dictionary*>(object);
        __Array* array = dict->allKeys();
        Ref* pObj;
        CCARRAY_FOREACH(array,pObj)
        {
            __String* key2 = (__String*)pObj;
            string nkey = key2->getCString();
            Ref* obj = (dict->objectForKey(nkey));
            if (dynamic_cast<__Integer *>(obj)) {
                data[nkey] = dynamic_cast<__Integer *>(obj)->getValue();
            }else if (dynamic_cast<__String *>(obj)) {
                data[nkey] = dynamic_cast<__String *>(obj)->getCString();
            }else if (dynamic_cast<__Float *>(obj)) {
                data[nkey] = dynamic_cast<__Float *>(obj)->getValue();
            }else if (dynamic_cast<__Double *>(obj)) {
                data[nkey] = dynamic_cast<__Double *>(obj)->getValue();
            }else if (dynamic_cast<__Bool *>(obj)) {
                data[nkey] = dynamic_cast<__Bool *>(obj)->getValue();
            }else if(dynamic_cast<__Array*>(obj)){
               
                __Array* pArray = dynamic_cast<__Array*>(obj);
                Ref* ppObj;
                vector<Ref*> array;
                CCARRAY_FOREACH(pArray,ppObj)
                {
                    array.push_back(ppObj);
                }
                Json::Value data1;
                array2Json(&data1,&array);
                data[nkey] = data1;
            }
        }
        
        (*value)[key] = data;
    }else if(dynamic_cast<__Array*>(object)){
        __Array* pArray = dynamic_cast<__Array*>(object);
        Ref* pObj;
        vector<Ref*> array;
        CCARRAY_FOREACH(pArray,pObj)
        {
            array.push_back(pObj);
        }
        Json::Value data;
        array2Json(&data,&array);
        (*value)[key] = data;
    }
}

void BmobJsonUtil::dictionary2Json(Json::Value *value,std::map<std::string, cocos2d::Ref *> *mapData){
    for (map<string, Ref*>::iterator it = (*mapData).begin(); it!=(*mapData).end(); ++it) {
        object2Json(value, it->first, it->second);
    }
}

void BmobJsonUtil::dictionary2Json_m(Json::Value *value,std::multimap<std::string, cocos2d::Ref *> *mapData){
    for (multimap<string, Ref*>::iterator it = (*mapData).begin(); it!=(*mapData).end(); ++it) {
        multimap<string, Ref*>::iterator beg = (*mapData).lower_bound(it->first);
        multimap<string, Ref*>::iterator end = (*mapData).upper_bound(it->first);
        map<string,Ref*> temp;
        __Array* noArray = NULL;
        Json::Value data;
        while(beg != end)
         {
            cout<<"................................"<<it->first<< endl;
            __Dictionary* dict = dynamic_cast<__Dictionary*>((*beg).second);
            if (dict)
            {
                /* code */
                __Array* array = dict->allKeys();
                __String* key = (__String*)(array->getObjectAtIndex(0));
                Ref* obj = (dict->objectForKey(key->getCString()));
                temp.insert(pair<string,Ref*>(key->getCString(),obj));
            }
            else if(dynamic_cast<__Array*>((*beg).second)){
                // if(array == NULL){
                //     array = __Array::create();
                // }
                // array->addObject((*beg).second);
            }else{
                if (noArray == NULL)
                {
                    /* code */
                    noArray = __Array::create();
                }

                noArray->addObject((*beg).second);
            }
            ++beg;
         } 
         if (noArray == NULL)
         {
             /* code */
            dictionary2Json(&data,&temp);
            (*value)[it->first] = data;
         }else{
            if (noArray->count() == 1)
            {
                /* code */
                Ref* obj = noArray->getObjectAtIndex(0);
                object2Json(value, it->first, obj);
            }else{
                object2Json(&data, it->first, noArray);
                (*value)[it->first] = data;
            }
         }
    }
}



void BmobJsonUtil::array2Json(Json::Value *jarray,vector<cocos2d::Ref *> *list){
    int count = (*list).size();
    for (int i = 0; i<count; ++i) {
        int size = (*jarray).size();
        Ref *object = (*list)[i];
        if (dynamic_cast<__Integer *>(object)) {
            (*jarray)[size] = dynamic_cast<__Integer *>(object)->getValue();
        }else if (dynamic_cast<__String *>(object)) {
            (*jarray)[size] = dynamic_cast<__String *>(object)->getCString();
        }else if (dynamic_cast<__Float *>(object)) {
            (*jarray)[size] = dynamic_cast<__Float *>(object)->getValue();
        }else if (dynamic_cast<__Double *>(object)) {
            (*jarray)[size] = dynamic_cast<__Double *>(object)->getValue();
        }else if (dynamic_cast<__Bool *>(object)) {
            (*jarray)[size] = dynamic_cast<__Bool *>(object)->getValue();
        }
    }
}


Ref* BmobJsonUtil::json2Object(Json::Value *value, std::string key){
    if ((*value)[key].isBool()) {
        return __Bool::create((*value)[key].asBool());
    }else if ((*value)[key].isString()) {
        return __String::create((*value)[key].asString());
    }else if ((*value)[key].isNumeric()) {
        return __Double::create((*value)[key].asInt());
    }else {
        // BmobParams *params = NULL;
        // if ((*value)[key].type() == Json::arrayValue){
        //     params = new BmobArray();
        // }else if ((*value)[key].type() == Json::objectValue){
        //     string type = (*value)[key]["__type"].asString();
        //     if (type == "") {
        //         params = new BmobObject();
        //     }else if (type == kTagParamTypeDate) {
        //         params = new BmobDate();
        //     }else if (type == kTagParamTypeGeopoint) {
        //         params = new BmobGeopoint(0,0);
        //     }else if (type == kTagParamTypeFile) {
        //         params = new BmobFile();
        //     }
        // }
        // if (params) {
        //     params->autorelease();
        //     params->deJson(&(*value)[key]);
        //     return params;
        // }
        
    }

    return NULL;
}

void BmobJsonUtil::json2Dictionary(Json::Value *value,std::map<std::string, cocos2d::Ref *> *mapData){
    Json::Value::Members members = (*value).getMemberNames();
    for (Json::Value::Members::iterator it = members.begin(); it != members.end(); ++it){
        string key = *it;
        Ref *object = json2Object(value, key);
        if (object) {
            (*mapData).insert(pair<string, Ref*>(key,object));
        }
    }
}

void BmobJsonUtil::json2Array(Json::Value *jarray, vector<cocos2d::Ref *> *list){
    int size = (*jarray).size();
    for (int i=0; i<size; ++i) {
        if ((*jarray)[i].isBool()) {
            (*list).push_back(__Bool::create((*jarray)[i].asBool()));
        }else if ((*jarray)[i].isString()) {
            (*list).push_back(__String::create((*jarray)[i].asString()));
        }else if ((*jarray)[i].isNumeric()) {
            (*list).push_back(__Double::create((*jarray)[i].asInt()));
        }
    }
}

string BmobJsonUtil::json2String(Json::Value *value){
    std::string data;
    Json::Value::Members members((*value).getMemberNames());
    for (Json::Value::Members::iterator it = members.begin(); it != members.end(); ++it)  {
        data+=(*it);
        data+="=";
        if ((*value)[*it].isNumeric()) {
            std::stringstream ss;
            ss<<(*value)[*it];
            data+=ss.str();
        }else{
            data+=(*value)[*it].asString();
        }
        data+="&";
    }
    
    data = data.substr(0,data.length()-1);
    
    return data;
}