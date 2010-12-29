-- ----------------------------
-- Table structure for `character_xp_rates`
-- ----------------------------
DROP TABLE IF EXISTS `character_xp_rates`;
CREATE TABLE `character_xp_rates` (
	`guid`  int(11) UNSIGNED NOT NULL DEFAULT 0 COMMENT 'Global Unique Identifier' ,
	`start_time`  timestamp NOT NULL DEFAULT '0000-00-00 00:00:00' COMMENT 'Absolute start date, the bonus will be granted' ,
	`end_time`  timestamp NOT NULL DEFAULT '0000-00-00 00:00:00' COMMENT 'Absolute end date, the bonus is expired' ,
	`kill_xp_rate`  int(11) UNSIGNED NOT NULL DEFAULT 1 COMMENT 'Experience Multiplier' ,
	`quest_xp_rate`  int(11) UNSIGNED NOT NULL DEFAULT 1 COMMENT 'Experience Multiplier' ,
	`explore_xp_rate`  int(11) UNSIGNED NOT NULL DEFAULT 1 COMMENT 'Experience Multiplier' ,
PRIMARY KEY (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- Use this query once, to copy all players in characters into character_xp_rate with default values
-- All characters thats got made after this patch is applied, got the values automatically
-- This query will override any entry in character_xp_rates, so just use it once after patching
INSERT IGNORE INTO `character_xp_rates` (`guid`) SELECT `guid` FROM `characters`;