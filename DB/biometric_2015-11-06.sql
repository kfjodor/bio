# ************************************************************
# Sequel Pro SQL dump
# Version 4499
#
# http://www.sequelpro.com/
# https://github.com/sequelpro/sequelpro
#
# Host: 192.168.3.3 (MySQL 5.5.44-0ubuntu0.12.04.1)
# Database: biometric
# Generation Time: 2015-11-06 10:23:31 +0000
# ************************************************************


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;


# Dump of table api_log
# ------------------------------------------------------------

DROP TABLE IF EXISTS `api_log`;

CREATE TABLE `api_log` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `ip` varchar(255) DEFAULT NULL,
  `created_at` datetime DEFAULT NULL,
  `updated_at` datetime DEFAULT NULL,
  `uid` varchar(255) DEFAULT NULL,
  `project_id` int(10) unsigned DEFAULT NULL,
  `error` int(11) DEFAULT '0',
  `request` text,
  `response` text,
  `uri` varchar(255) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `api_log_uid_index` (`uid`),
  KEY `api_log_project_id_index` (`project_id`),
  CONSTRAINT `api_log_project_id_foreign` FOREIGN KEY (`project_id`) REFERENCES `project` (`id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

LOCK TABLES `api_log` WRITE;
/*!40000 ALTER TABLE `api_log` DISABLE KEYS */;

INSERT INTO `api_log` (`id`, `ip`, `created_at`, `updated_at`, `uid`, `project_id`, `error`, `request`, `response`, `uri`)
VALUES
	(1,'11010817','2015-11-05 06:44:22','2015-11-05 06:44:22',NULL,1,0,'{\"uuid\":100,\"project\":1,\"secret\":\"6cfdbef1d0c491373b1964d3774f1a032903304c\"}','{\"uuid\":100,\"errorCode\":0,\"errorMessage\":null,\"response\":{\"id\":5}}',''),
	(2,'11010817','2015-11-05 06:44:23','2015-11-05 06:44:23',NULL,1,0,'{\"uuid\":100,\"project\":1,\"secret\":\"6cfdbef1d0c491373b1964d3774f1a032903304c\"}','{\"uuid\":100,\"errorCode\":0,\"errorMessage\":null,\"response\":{\"id\":6}}',''),
	(3,'11010817','2015-11-05 06:45:33','2015-11-05 06:45:33','100',1,0,'{\"uuid\":100,\"project\":1,\"secret\":\"6cfdbef1d0c491373b1964d3774f1a032903304c\"}','{\"uuid\":100,\"errorCode\":0,\"errorMessage\":null,\"response\":{\"id\":7}}',''),
	(4,'11010817','2015-11-05 06:45:33','2015-11-05 06:45:33','100',1,0,'{\"uuid\":100,\"project\":1,\"secret\":\"6cfdbef1d0c491373b1964d3774f1a032903304c\"}','{\"uuid\":100,\"errorCode\":0,\"errorMessage\":null,\"response\":{\"id\":8}}',''),
	(5,'11010817','2015-11-05 08:02:30','2015-11-05 08:02:30','100',1,0,'{\"uuid\":100,\"project\":1,\"secret\":\"6cfdbef1d0c491373b1964d3774f1a032903304c\"}','{\"uuid\":100,\"errorCode\":0,\"errorMessage\":null,\"response\":{\"id\":9}}',''),
	(6,'11010817','2015-11-05 08:51:19','2015-11-05 08:51:19','100',1,0,'{\"uuid\":100,\"project\":1,\"secret\":\"6cfdbef1d0c491373b1964d3774f1a032903304c\"}','{\"uuid\":100,\"errorCode\":0,\"errorMessage\":null,\"response\":{\"id\":10}}','/user/create'),
	(17,'11010817','2015-11-05 14:20:44','2015-11-05 14:20:44',NULL,NULL,NULL,'{\"uuid\":100,\"project\":1,\"secret\":\"f9ddd3839d109ea669eb4308dd05beedc29f3739\",\"user_id\":12,\"template\":\"abcabc\"}','{\"uuid\":100,\"errorCode\":1,\"errorMessage\":\"wrong secret probhited\",\"response\":{}}','/fingerprint/create'),
	(23,'11010817','2015-11-05 14:24:04','2015-11-05 14:24:04','100',1,0,'{\"uuid\":100,\"project\":1,\"secret\":\"6cfdbef1d0c491373b1964d3774f1a032903304c\"}','{\"uuid\":100,\"errorCode\":0,\"errorMessage\":null,\"response\":{}}','/fingerprint/delete/3');

/*!40000 ALTER TABLE `api_log` ENABLE KEYS */;
UNLOCK TABLES;


# Dump of table fingerprint
# ------------------------------------------------------------

DROP TABLE IF EXISTS `fingerprint`;

CREATE TABLE `fingerprint` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `user_id` bigint(20) unsigned NOT NULL,
  `template` blob NOT NULL,
  `created_at` datetime DEFAULT NULL,
  `updated_at` datetime DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `fingerprint_user_id_foreign` (`user_id`),
  CONSTRAINT `fingerprint_user_id_foreign` FOREIGN KEY (`user_id`) REFERENCES `user` (`id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

LOCK TABLES `fingerprint` WRITE;
/*!40000 ALTER TABLE `fingerprint` DISABLE KEYS */;

INSERT INTO `fingerprint` (`id`, `user_id`, `template`, `created_at`, `updated_at`)
VALUES
	(1,12,X'616263616263','2015-11-05 14:20:50','2015-11-05 14:20:50'),
	(2,12,X'616263616263','2015-11-05 14:21:42','2015-11-05 14:21:42');

/*!40000 ALTER TABLE `fingerprint` ENABLE KEYS */;
UNLOCK TABLES;


# Dump of table project
# ------------------------------------------------------------

DROP TABLE IF EXISTS `project`;

CREATE TABLE `project` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `name` varchar(255) NOT NULL,
  `passphrase` varchar(255) NOT NULL,
  `active` tinyint(1) NOT NULL DEFAULT '1',
  `biometric_host` varchar(255) DEFAULT '127.0.0.1',
  `biometric_port` varchar(255) DEFAULT '6060',
  PRIMARY KEY (`id`),
  UNIQUE KEY `project_name_unique` (`name`),
  UNIQUE KEY `project_passphrase_unique` (`passphrase`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

LOCK TABLES `project` WRITE;
/*!40000 ALTER TABLE `project` DISABLE KEYS */;

INSERT INTO `project` (`id`, `name`, `passphrase`, `active`, `biometric_host`, `biometric_port`)
VALUES
	(1,'Test','testing',1,'127.0.0.1','6060'),
	(2,'Хуестинг','testing2',0,'127.0.0.1','6060'),
	(3,'undefined','',1,'127.0.0.1','6060'),
	(7,'asdasdasdasd','ZsYRGZtk2JqvcG2F',1,'127.0.0.1','6060'),
	(8,'xczczxc','27rnDPvLQwt5xDL8',1,'127.0.0.1','6060'),
	(9,'asdadasd','Q2V4DbUwvsm7vEMG',1,'127.0.0.1','6060'),
	(10,'123123123','UVys84VkrjSiH7Su',1,'127.0.0.1','6060'),
	(13,'21312312312312','LNaFdp0VO2iiZzYW',1,'127.0.0.1','6060'),
	(14,'adsadasdasd','rlaivuUwiF4QQOM5',1,'127.0.0.1','6060'),
	(15,'zxccxzcxzcxz','sMFHcla3HFlY5bVk',1,'127.0.0.1','6060'),
	(16,'asdsdadasdxzczxc','QYJ3JV6LmSWGBlF9',1,'127.0.0.1','6060'),
	(17,'dzczaeadsdas','fcwyssj9xbN2e5cC',1,'127.0.0.1','6060'),
	(18,'Большой проект','QxkdzByFsZsBXYwrs',1,'127.0.0.1','6060');

/*!40000 ALTER TABLE `project` ENABLE KEYS */;
UNLOCK TABLES;


# Dump of table project_device
# ------------------------------------------------------------

DROP TABLE IF EXISTS `project_device`;

CREATE TABLE `project_device` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `project_id` int(10) unsigned NOT NULL,
  `serial` varchar(255) NOT NULL,
  `passphrase` varchar(255) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `project_device_project_id_foreign` (`project_id`),
  CONSTRAINT `project_device_project_id_foreign` FOREIGN KEY (`project_id`) REFERENCES `project` (`id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

LOCK TABLES `project_device` WRITE;
/*!40000 ALTER TABLE `project_device` DISABLE KEYS */;

INSERT INTO `project_device` (`id`, `project_id`, `serial`, `passphrase`)
VALUES
	(4,18,'asdsadasd','asdasdsadasdasd'),
	(5,18,'czxcc','xczxczxczxc'),
	(6,18,'saddasdasd','asdasdasdasda'),
	(7,18,'zxcczcxz','asdalsdas;lkd'),
	(8,18,'asdl;asd;as;dkla','sd;ladk;salkd'),
	(9,18,'czx;lkc;zlkcx;','a;lskdl;askdl;askd'),
	(10,18,'asdl;kaskdlalkd',';dsa;dasd;lasdl;kas'),
	(11,18,'asdkl;sakd;askd;cxz','as;lkdaldk;lasdlkas'),
	(12,18,'as;ldas;ldasl;d;lk','as;dlkas;ldkasl;kdl;kas'),
	(13,18,'sad;lkasl;dkasl;kdlk;','asl;dkas;ldals;dlkas;'),
	(14,18,'asdlkaldjaslkdjaksl','dlaskjdlkasdlkasjlkda'),
	(15,18,'aslkdjaskljdlaskjd','aslkdaslkdjlkasdkljas'),
	(16,18,'asdlkjaslkdjaslkjdaskjdjkl','jaskjdlaskjdlaskjdakls'),
	(17,18,'asdlkjalkdasjlkdjaslsadads','dlkjasdlkjaskldasjdlkasj');

/*!40000 ALTER TABLE `project_device` ENABLE KEYS */;
UNLOCK TABLES;


# Dump of table project_log
# ------------------------------------------------------------

DROP TABLE IF EXISTS `project_log`;

CREATE TABLE `project_log` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `project_id` int(10) unsigned NOT NULL,
  `level_id` int(10) unsigned NOT NULL,
  `message` varchar(255) NOT NULL,
  `created_at` datetime DEFAULT NULL,
  `updated_at` datetime DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `project_log_project_id_foreign` (`project_id`),
  KEY `project_log_level_id_foreign` (`level_id`),
  CONSTRAINT `project_log_level_id_foreign` FOREIGN KEY (`level_id`) REFERENCES `severity_level` (`id`) ON DELETE CASCADE,
  CONSTRAINT `project_log_project_id_foreign` FOREIGN KEY (`project_id`) REFERENCES `project` (`id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;



# Dump of table severity_level
# ------------------------------------------------------------

DROP TABLE IF EXISTS `severity_level`;

CREATE TABLE `severity_level` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `name` varchar(255) NOT NULL,
  `description` varchar(255) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `severity_level_name_unique` (`name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

LOCK TABLES `severity_level` WRITE;
/*!40000 ALTER TABLE `severity_level` DISABLE KEYS */;

INSERT INTO `severity_level` (`id`, `name`, `description`)
VALUES
	(1,'debug','DEBUG'),
	(2,'notice','NOTICE'),
	(3,'warning','WARNING'),
	(4,'error','ERROR');

/*!40000 ALTER TABLE `severity_level` ENABLE KEYS */;
UNLOCK TABLES;


# Dump of table user
# ------------------------------------------------------------

DROP TABLE IF EXISTS `user`;

CREATE TABLE `user` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `project_id` int(10) unsigned NOT NULL,
  `created_at` datetime DEFAULT NULL,
  `updated_at` datetime DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `user_project_id_foreign` (`project_id`),
  CONSTRAINT `user_project_id_foreign` FOREIGN KEY (`project_id`) REFERENCES `project` (`id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

LOCK TABLES `user` WRITE;
/*!40000 ALTER TABLE `user` DISABLE KEYS */;

INSERT INTO `user` (`id`, `project_id`, `created_at`, `updated_at`)
VALUES
	(1,1,'2015-11-05 06:42:03','2015-11-05 06:42:03'),
	(2,1,'2015-11-05 06:42:38','2015-11-05 06:42:38'),
	(3,1,'2015-11-05 06:42:51','2015-11-05 06:42:51'),
	(4,1,'2015-11-05 06:43:58','2015-11-05 06:43:58'),
	(5,1,'2015-11-05 06:44:22','2015-11-05 06:44:22'),
	(6,1,'2015-11-05 06:44:23','2015-11-05 06:44:23'),
	(7,1,'2015-11-05 06:45:33','2015-11-05 06:45:33'),
	(8,1,'2015-11-05 06:45:33','2015-11-05 06:45:33'),
	(9,1,'2015-11-05 08:02:29','2015-11-05 08:02:29'),
	(10,1,'2015-11-05 08:51:19','2015-11-05 08:51:19'),
	(12,1,'2015-11-05 14:20:22','2015-11-05 14:20:22');

/*!40000 ALTER TABLE `user` ENABLE KEYS */;
UNLOCK TABLES;



/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
