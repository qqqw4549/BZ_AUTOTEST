#include "userhandler.h"

void UserHandler::info(const http::Request &req, http::Response &res)
{
    json permission = {
        { "roleId", "admin" },
        { "permissionId", "user" },
        { "permissionName", "用户管理" },
        { "id", "admin" }
    };
    json role = {
        { "id", "admin" },
        { "name", "管理员" },
        { "describe", "拥有所有权限" },
        { "status", 1 },
        { "creatorId", "system" },
        { "createTime", 1497160610259 },
        { "permissions", { permission } },
    };
    json data =  {
            { "id", "xxxxxxxxxxxxuyyy" },
            { "name", "管理员" },
            { "username", "admin" },
            { "password", "" },
            { "avatar", "avatar2.jpg" },
            { "status", 1 },
            { "telephone", "" },
            { "lastLoginIp", 1497160610259},
            { "lastLoginTime", "1534837621348" },
            { "creatorId", "admin" },
            { "createTime", 1497160610259 },
            { "merchantCode", "TLif2btpzg079h15bk" },
            { "deleted", 0 },
            { "roleId", "admin" },
            { "role", role }
    };
    res.set_content(body(data), "application/json");
}

void UserHandler::nav(const http::Request &req, http::Response &res)
{
    json data =  R"( 
        [

            {
                "name":"config",
                "parentId":0,
                "id":10,
                "meta":{
                    "icon":"setting",
                    "title":"配置页",
                    "show":true
                },
                "component":"RouteView",
                "redirect":"/config/base-config"
            },
            {
                "name":"base-config",
                "parentId":10,
                "id":11,
                "meta":{
                    "title":"基本配置",
                    "show":true
                },
                "component":"BaseConfig"
            },
            {
                "name":"calibration-config",
                "parentId":10,
                "id":12,
                "meta":{
                    "title":"校准配置",
                    "show":true
                },
                "component":"CalibrationConfig"
            },
            {
                "name":"jesd204b-config",
                "parentId":10,
                "id":13,
                "meta":{
                    "title":"204B配置",
                    "show":true
                },
                "component":"Jesd204bConfig"
            },
            {
                "name":"gpio-config",
                "parentId":10,
                "id":13,
                "meta":{
                    "title":"GPIO配置",
                    "show":true
                },
                "component":"GpioConfig"
            },
            {
                "name":"agc-config",
                "parentId":10,
                "id":13,
                "meta":{
                    "title":"AGC配置",
                    "show":true
                },
                "component":"AgcConfig"
            },
            {
                "name":"rxdata",
                "parentId":0,
                "id":20,
                "meta":{
                    "icon":"dot-chart",
                    "title":"采数页",
                    "show":true
                },
                "component":"RouteView",
                "redirect":"/rxdata/base-orxdata"
            },
            {
                "name":"base-orxdata",
                "parentId":20,
                "id":21,
                "meta":{
                    "title":"ORX采数",
                    "show":true
                },
                "component":"BaseORxData"
            },
            {
                "name":"base-rxdata",
                "parentId":20,
                "id":22,
                "meta":{
                    "title":"RX采数",
                    "show":true
                },
                "component":"BaseRxData"
            },
            {
                "name":"txdata",
                "parentId":0,
                "id":30,
                "meta":{
                    "icon":"line-chart",
                    "title":"发数页",
                    "show":true
                },
                "component":"BaseTxData"
            },
            {
                "name":"function",
                "parentId":0,
                "id":40,
                "meta":{
                    "icon":"bars",
                    "title":"功能页",
                    "show":true
                },
                "component":"BaseFunction"
            },
            {
                "name":"base-register",
                "parentId":0,
                "id":50,
                "meta":{
                    "icon":"trademark",
                    "title":"寄存器",
                    "show":true
                },
                "component":"BaseReg"
            },
            {
                "name":"account",
                "parentId":0,
                "id":10028,
                "meta":{
                    "title":"个人页",
                    "icon":"user",
                    "show":true
                },
                "redirect":"/account/center",
                "component":"RouteView"
            },
            {
                "name":"settings",
                "parentId":10028,
                "id":10030,
                "meta":{
                    "title":"个人设置",
                    "hideHeader":true,
                    "hideChildren":true,
                    "show":true
                },
                "redirect":"/account/settings/basic",
                "component":"AccountSettings"
            },
            {
                "name":"BasicSettings",
                "path":"/account/settings/basic",
                "parentId":10030,
                "id":10031,
                "meta":{
                    "title":"基本设置",
                    "show":false
                },
                "component":"BasicSetting"
            },
            {
                "name":"SecuritySettings",
                "path":"/account/settings/security",
                "parentId":10030,
                "id":10032,
                "meta":{
                    "title":"安全设置",
                    "show":false
                },
                "component":"SecuritySettings"
            },
            {
                "name":"CustomSettings",
                "path":"/account/settings/custom",
                "parentId":10030,
                "id":10033,
                "meta":{
                    "title":"个性化设置",
                    "show":false
                },
                "component":"CustomSettings"
            },
            {
                "name":"BindingSettings",
                "path":"/account/settings/binding",
                "parentId":10030,
                "id":10034,
                "meta":{
                    "title":"账户绑定",
                    "show":false
                },
                "component":"BindingSettings"
            },
            {
                "name":"NotificationSettings",
                "path":"/account/settings/notification",
                "parentId":10030,
                "id":10034,
                "meta":{
                    "title":"新消息通知",
                    "show":false
                },
                "component":"NotificationSettings"
            }
        ]
    )"_json;
    res.set_content(body(data), "application/json");
}