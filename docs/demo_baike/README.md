# ǰ��
--------
�����԰ٶȰٿ�Ϊ���ӣ����ܹ����̨��ʹ�ã���ngx-pms Agent����˵��

# Ŀ¼
* [���Ӧ��](#���Ӧ��)
* [�����ͨ�û�](#�����ͨ�û�)
* [����Ȩ�޹���](#����Ȩ�޹���)
* [�ٿƴ�������](#�ٿƴ�������)
* [�����û�](#�����û�)

##### ���Ӧ��
---------------
* ������root��¼�����̨,Ȼ�����Ҳ��<Ӧ�ù���>���������£�<br/>
![Ӧ���б�](imgs/app_list.png)
* Ȼ����<���>������Ӧ����ӵĽ��棬���������Ϣ��<br/>
![Ӧ�����](imgs/app_add.png)
* ���<����>������ɹ��󣬻ᵯ��������ʾ��<br/>
![Ӧ����ӳɹ�](imgs/app_add_success.png)<br/>
*���סӦ�ù���Ա�ʺż����롣*

##### �����ͨ�û�
-----------------
* ���û������У����<���>���ڶԻ�����������Ӧ���û��������䣺<br/>
![add_user_baike01](imgs/add_user_baike01.png)<br/>
���<����>���û�����ӳɹ��ˡ���ע�Ᵽ�����ʾ����������롣

##### ����Ȩ�޹���
-----------------------
Ӧ����ӳɹ�����Ҫ��Ӧ�ù���Ա��ݵ�¼�����̨��������صĹ���*root�ʺ�ֻ�����Ӧ�ü��û������ܽ���Ȩ�޼�URL����*

* �԰ٿƵ��ʺ�`baike`��¼����ϵͳ��<br/>
![baike��¼](imgs/baike_login.png)<br/>
* ��¼�󣬵�����Ͻǵ�<�޸�����>�����޸�baike�����룺
![baike�޸�����](imgs/baike_change_pwd.png)<br/>
* ��URL�����У����Ե��<���>�����һ���µ�URL��<br/>

�����һ��URLΪ`/`,ƥ������Ϊ`ǰ׺ƥ��`������Ȩ��Ϊ`�����˲��ɷ���`��URL������URL����/��ͷ���������URL��ƥ������`δƥ��������URL��URL`�����URL���õ���˼������δ���õ�URL�������˶����ɷ���<br/>
![Ĭ��URL](imgs/url_def.png)<br/>

�����һ��URLΪ`/`,ƥ������Ϊ`��ȷƥ��`������Ȩ��Ϊ`�����˿ɷ���`��URL�����URL����˼�����е�¼�û����ɷ��ʰٿƵ���ҳ��<br/>
![Ĭ��URL](imgs/url_index.png)<br/>

����һ�ڽ����ڷ������������pms��Agent��

##### �ٿƴ�������
pms agent��Ҫ�����ڷ�������ϣ�agent���������¡�

* �������pms Agent�������£�

```nginx
lua_package_path "/data/ngx-pms/?.lua;;";
server {
    listen       88 default; 

    # ����ngx-pms�����ϡ�
    location /nright {
        proxy_set_header Host $host;
        proxy_set_header X-Real-IP $remote_addr;
        proxy_set_header REMOTE-HOST $remote_addr;
        proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
        proxy_set_header Accept-Encoding "";
        client_max_body_size 50m;
        client_body_buffer_size 256k;
        # ָ�򵽸ղ����PMS��Ȩ�����ϡ�
        proxy_pass    http://127.0.0.1:8000;
    }

    # �����Ӧ���С��
    header_filter_by_lua ' ngx.header.content_length = nil '; 
    # ������������Ӧҳ�棬������Ϣ����
    body_filter_by_lua_file /data/ngx-pms/agent/body_filter.lua;
    # Ӧ�õķ���������á�
    location / {
        # $app������ֵ���������Ѿ��ڹ����̨���Ѿ���ӳɹ���Ӧ��ID.
        set $app baike;
        # Ȩ�޼��Ľű���
        access_by_lua_file /data/ngx-pms/agent/right_check.lua;

        proxy_set_header Host "baike.baidu.com";
        proxy_set_header X-Real-IP $remote_addr;
        proxy_set_header REMOTE-HOST $remote_addr;
        proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
        proxy_set_header Accept-Encoding "";
        # baike��ʵ�ʵ�ַ��
        proxy_pass   http://119.75.222.21;
    }
}
```
���ú�֮�󣬾Ϳ���������������ˡ�Ȼ��Ϳ���ͨ�����������ʰٿ��ˡ�

* �ڿͻ����ϣ���baike.baidu.com ��������������ϣ�ͨ��hosts����Ȼ�����`http://baike.baidu.com`������û�е�¼��ͳһȨ�޹���ϵͳ����ת����¼ҳ�棺
![��¼](imgs/pms_login.png)<br/>
* �����ʺż�����󣬾ͽ��뵽�˰ٶȰٿ���ҳ���ˣ�<br/>
![�ٿ���ҳ](imgs/baike_index.png)<br/>
ע�⿴��ҳ�����������һ����������������ʾ��ǰ�û����޸����룬�ǳ���ѡ�

* �����ʵ�ҳ��ûȨ��ʱ����������������ҳ�棺
![ûȨ��](imgs/no_access_page.png)<br/>

##### �����û�
�û��� | ���� | Ȩ�� 
----- | ---- | --- 
baike01 | 123456 | ��������ҳȨ��
baike02 | 123456 | ��ҳ���������Ƽ�����Ȼ���Ļ�����ʷ�ϵĽ��죬����ţ��ٿ�����
baike03 | 123456 | baike01,baike02����Ȩ�ޣ����ϴ���Ȩ��(��/item��ͷ),��/view��ͷ��ҳ��Ȩ�ޡ�

* ���Է��������baike.baidu.com ����ָ����114.215.210.244������hosts����ӣ�

```shell
114.215.210.244 baike.baidu.com
```
* Ȼ�����baike.baidu.com��ע�⣺�����û������޸����롣