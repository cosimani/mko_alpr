-- sqlite script

pragma foreign_keys = off;
begin transaction;

-- table: plate_detections
drop table if exists plate_detections;

create table plate_detections (
    id            integer not null,
    session_id    integer not null,
    constraint pk__plate_detections primary key (
        id
    ),
    constraint fk__plate_detections__sessions__1 foreign key (
        session_id
    )
    references sessions (id) 
);

-- table: logs
drop table if exists logs;

create table logs (
    id         integer        not null,
    time       datetime       not null,
    message    varchar (1000) not null,
    level_id   integer        not null,
    constraint pk__logs primary key (
        id
    ),
    constraint fk__logs__levels__1 foreign key (
        level_id
    )
    references levels (id),
    constraint ch__logs__message__1 check (message <> '') 
);

-- table: sessions
drop table if exists sessions;

create table sessions (
    id       integer  not null,
    start    datetime not null,
    admin_id integer  not null,
    constraint pk__sessions primary key (
        id
    ),
    constraint fk__sessions__admins__1 foreign key (
        admin_id
    )
    references admins (id) 
);

-- table: levels
drop table if exists levels;

create table levels (
    id        integer       not null,
    name      varchar (255) not null,
    criticaly tinyint (1)   not null,
    constraint pk__levels primary key (
        id
    ),
    constraint uq__levels__1 unique (
        name
    ),
    constraint ch__levels__name__1 check (name <> ''),
    constraint ch__levels__criticaly__1 check (criticaly in (0, 1) ) 
);

-- table: candidates
drop table if exists candidates;

create table candidates (
    id                 integer       not null,
    domain             varchar (255) not null,
    matches            char(1)       not null,
    confidence         float (7, 4)  not null,
    plate_detection_id integer       not null,
    constraint pk__candidates primary key (
        id
    ),
    constraint fk__candidates__plate_detections__1 foreign key (
        plate_detection_id
    )
    references plate_detections (id),
    constraint ch__candidates__domain__1 check (domain <> ''),
    constraint ch__candidates__matches__1 check (matches in ('s', 'n')),
    constraint ch__candidates__confidence__1 check (confidence >= 0),
    constraint ch__candidates__confidence__2 check (confidence <= 100) 
);

-- table: admins
drop table if exists admins;

create table admins (
    id       integer       not null,
    user     varchar (255) not null,
    password varchar (255) not null,
    constraint pk__admins primary key (
        id
    ),
    constraint uq__admins__1 unique (
        user
    ),
    constraint ch__admins__user__1 check (user <> ''),
    constraint ch__admins__password__1 check (user <> '') 
);

commit transaction;
pragma foreign_keys = on;

-- sqlite script

insert into levels (name, criticaly)
       values ('inf', 0),
              ('war', 0),
              ('err', 1);

insert into admins (user, password)
       values ('cesar', '6f597c1ddab467f7bf5498aad1b41899'),
              ('emi', '12b41c761b41698d39ef68fdd9429578'),
              ('nano', '1657ec96792937f71c20c9e1bdc2300f');