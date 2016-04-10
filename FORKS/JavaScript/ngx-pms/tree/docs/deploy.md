
#�Ⱦ����� 
* ��װMYSQL
* ��װOpenResty

# Ŀ¼
* [��װ](#��װ)
    * [���ݿ��ʼ��](#���ݿ��ʼ��)
    * [������](#������)
    * [��Ӹ��ʺ�](#��Ӹ��ʺ�)
* [ʹ�ù����̨](#ʹ�ù����̨)
* [����Ȩ�޴���](#����Ȩ�޴���)
* [����Ӧ�ò���¼](#����Ӧ�ò���¼)
* [�ٶȰٿ�����ʾ��](demo_baike/README.md)

# ��װ
-------
#### ���ݿ��ʼ��
* �ȴ������ݿ⣬`����mysql�����У�ִ���������`

```shell
create database pms character set utf8;
use pms;
```

* ִ�нű�������ṹ��

```shell
source /path/to/ngx-pms/docs/scripts/db.sql
```
���ִ�гɹ���Ӧ�ô��������¼��ű�
```mysql
mysql> show tables;
+-----------------+
| Tables_in_pms   |
+-----------------+
| application     |
| permission      |
| role            |
| url_perm        |
| user            |
| user_permission |
+-----------------+
6 rows in set (0.00 sec)
```

#### ������
* nginx.conf����

```nginx
    #����cookie�Ĺ����ڴ档
    lua_shared_dict cookies 5m;
    lua_package_path "/path/to/ngx-pms/?.lua;;";
    # ngx-pms��Ȩ�ӿ�
    server {
        listen 8000;
        set $template_root /path/to/ngx-pms/tmpl;

        location /nright/ {
            content_by_lua_file /path/to/ngx-pms/server/nright_main.lua;
        }
    }

    # ngx-pms�����̨��
    server {
        listen 8001;
        set $template_root /path/to/ngx-pms/Manager/templates;

        location / {
            content_by_lua_file /path/to/ngx-pms/Manager/lua/main.lua;
        }

        location /static {
            root /path/to/ngx-pms/Manager;
        }

        location = /password {
            allow 127.0.0.1;
            deny all;
            content_by_lua '
                local util = require("util.util")
                ngx.say(util.make_pwd(ngx.var.arg_password))
            ';
        }
    }
```

* config.lua����

```lua
-- ���沿��ʡ�ԡ�
-- Cookie ������ز�����
_M.cookie_config = {key="nright", path="/", expires=3600}

-- ���ݿ����á�
_M.db = {host="127.0.0.1", port=3306,user="root", password="123456",
		database="pms",DEFAULT_CHARSET="utf8"}

-- �б���ʾʱ��Ĭ�Ϸ�ҳ��С
_M.defNumPerPage = 15

-- Password����ʹ�õ��Σ���ϵͳʹ��֮ǰ�޸ģ�ϵͳ��ʼʹ�ú��벻Ҫ�޸ġ�
_M.password_magic = '#*nright@0Ol1llOO'
```
��Ҫ��Ҫ�޸�_M.db�е����ݿⲿ�֡�
ϵͳ��ʼ��ʱ����Ҫ�޸�_M.password_magic���֡�

* ����nginx<br/>
nginx�����󣬾Ϳ��Է��ʹ����̨����ϵͳ���й����ˣ������ڻ�û���ʺţ��޷���¼���밴������Ĳ����������룬�����root�ʺţ�

#### ��Ӹ��ʺ�
* �ڲ���ķ������ϣ�ʹ��������������ɼ��ܵ����룺<br/>
`curl http://127.0.0.1:8001/password?password=password-of-root`<br/>
���ִ�гɹ����᷵��һ��40λ�����룬���ƣ�`4b3fc94423693eab92d632688037f9d2`<br/>
���������鿴������־�����¶�λ�������ڡ�
* ʹ�ø����ɵ������滻�����${password}��Ȼ�������ݿ���ִ�У�<br/>
ģ��Ϊ��

```sql
insert into user(username,email,tel,password,app,manager, role_id,create_time, update_time)
values('root', 'jie123108@163.com', '13380333333', '${password}',
'all', 'super', '',unix_timestamp(), unix_timestamp());
```

�滻��Ϊ��<br/>

```sql
insert into user(username,email,tel,password,app,manager, role_id,create_time, update_time)
values('root', 'jie123108@163.com', '13380333333', '4b3fc94423693eab92d632688037f9d2',
'all', 'super', '',unix_timestamp(), unix_timestamp());
```

* ��Ӻ��ʺź�,�Ϳ��Է��ʹ����̨��������ϵͳ���й����ˡ�

# ʹ�ù����̨
--------
* ���ʸղŲ���Ĺ����̨���� `http://192.168.1.xx:8001/`
 ![��¼](login.png)
* ����root�������õ����룬�Ϳ��Ե�¼�ɹ��ˡ���ȥ�󣬽���������£�
 ![������](default.png)
* �����Ӧ�Ĳ˵������ܽ�����Ӧ�Ĺ����ˡ�

# ����Ȩ�޴���
--------------
Ȩ�޴���pms-agent������Ҫ������Ҫ�����Ӧ�õķ������nginx���棺

#### nginx.conf����
```nginx
lua_package_path "/path/to/ngx-pms/?.lua;;";
server {
    listen       80 default; 

    # ����ngx-pms�����ϡ�
    location /nright {
        proxy_set_header Host $host;
        proxy_set_header X-Real-IP $remote_addr;
        proxy_set_header REMOTE-HOST $remote_addr;
        proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
        proxy_set_header Accept-Encoding "";
        client_max_body_size 50m;
        client_body_buffer_size 256k;
        proxy_connect_timeout 10;
        proxy_send_timeout 10;
        proxy_read_timeout 10;
        proxy_buffer_size 256k;
        proxy_buffers 4 256k;
        proxy_busy_buffers_size 256k;
        proxy_temp_file_write_size 256k;
        proxy_max_temp_file_size 128m;
        # ָ�򵽸ղ���ķ����ϡ�
        proxy_pass    http://127.0.0.1:8000;
    }

    # �����Ӧ���С��
    header_filter_by_lua ' ngx.header.content_length = nil '; 
    # ������������Ӧҳ�棬������Ϣ����
    body_filter_by_lua_file /path/to/ngx-pms/agent/body_filter.lua;
    
    # Ӧ�õķ���������á�
    location / {
        # $app������ֵ���������Ѿ��ڹ����̨���Ѿ���ӳɹ���Ӧ��ID.
        set $app app_id;
        # Ȩ�޼��Ľű���
        access_by_lua_file /path/to/ngx-pms/agent/right_check.lua;

        proxy_set_header Host $host;
        proxy_set_header X-Real-IP $remote_addr;
        proxy_set_header REMOTE-HOST $remote_addr;
        proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
        proxy_set_header Accept-Encoding "";
        client_max_body_size 50m;
        client_body_buffer_size 256k;
        proxy_connect_timeout 10;
        proxy_send_timeout 10;
        proxy_read_timeout 10;
        proxy_buffer_size 256k;
        proxy_buffers 4 256k;
        proxy_busy_buffers_size 256k;
        proxy_temp_file_write_size 256k;
        proxy_max_temp_file_size 128m;
        # Ӧ�õ�ʵ�ʵ�ַ��
        proxy_pass    http://xx.xx.xx.xx:80;
    }
 }
```

# ����Ӧ�ò���¼
------
����Ӧ�ú󣬻���ʾ��¼����ʹ���ڹ����̨����ӵ��û��ʺŵ�¼���Ϳ��Է�����Ӧ����Ȩ�޵�ҳ���ˡ�
