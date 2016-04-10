
## create database pms character set utf8;
## use pms;

create table `application` (
app varchar(64) primary key comment '应用标识',
appname varchar(128) not null comment '应用名称',
remark varchar(1024) null comment '说明',
create_time integer unsigned comment '创建时间',
update_time integer unsigned comment '修改时间',
unique key(appname)
)engine=innodb comment='应用信息表';

create table `user` (
id integer primary key auto_increment,
username varchar(64) not null comment '用户名',
email varchar(128) not null comment '用户的email地址',
tel varchar(16) null comment '用户的手机号',
password varchar(64) not null comment '密码的md5值',
app varchar(64) not null comment '应用标识',
manager varchar(32) null comment '管理权限: super,超级管理员;admin,管理员',
role_id varchar(32) null comment '角色ID，只能有一个角色',
create_time integer unsigned comment '创建时间',
update_time integer unsigned comment '修改时间',
unique key(username),
key(email),
key(tel)
) engine=innodb comment='用户表';

create table `permission` (
id varchar(32) comment '权限ID,使用字母,下划线',
name varchar(64) not null comment '权限名称',
remark varchar(128) null comment '备注说明',
app varchar(64) not null comment '应用标识',
create_time integer unsigned comment '创建时间',
update_time integer unsigned comment '修改时间',
primary key(id),
unique key(name),
key(app)
) engine=innodb comment='权限表';

create table `url_perm` (
id bigint unsigned auto_increment,
app varchar(64) not null comment '应用标识',
type varchar(16) not null comment 'url匹配类型包含以下几种：
1.equal 精确匹配
2.suffix 后缀匹配
3.prefix 前缀匹配(最大匹配原则)
4.regex 正则匹配(后期支持)
匹配时，equal优先匹配，未匹配上时，
使用suffix匹配，然后是prefix，最后是regex
',
url varchar(256) not null comment 'URL',
url_len smallint default 0 comment 'url长度, 用作优先级，匹配时，
使用最大匹配原则，所以长度越大，优先级越高',
permission varchar(32) null comment '访问需要的权限，NULL表示任何人可访问',
create_time integer unsigned comment '创建时间',
update_time integer unsigned comment '修改时间',
primary key(id),
unique key(app,type,url(200))
) engine=innodb comment='URL权限表';

create table `role` (
id varchar(32) primary key comment '字符串类型的角色ID',
name varchar(64) not null comment '权限名称',
remark varchar(128) null comment '备注说明',
app varchar(64) not null comment '应用标识',
permission varchar(4096) null comment '权限列表，使用|分割',
create_time integer unsigned comment '创建时间',
update_time integer unsigned comment '修改时间',
unique key(name)
) engine=innodb comment='角色表';

create table `user_permission` (
userid integer not null comment '用户ID',
app varchar(64) not null comment '应用标识',
permission varchar(4096) null comment '权限列表，使用|分割',
create_time integer unsigned comment '创建时间',
update_time integer unsigned comment '修改时间',
primary key(userid, app)
) engine=innodb comment='用户权限表'

/**
insert into user(username,email,tel,password,app,manager, role_id,create_time, update_time)
values('root', 'root@163.com', '13380333333', '8507f087563cf8779138670fe0ad1f36',
'all', 'super', '',unix_timestamp(), unix_timestamp());
**/
