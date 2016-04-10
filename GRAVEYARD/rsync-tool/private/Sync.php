<?php
/*
*   Copyright 2012 Brandon Beveridge
*
*   Licensed under the Apache License, Version 2.0 (the "License");
*   you may not use this file except in compliance with the License.
*   You may obtain a copy of the License at
*
*       http://www.apache.org/licenses/LICENSE-2.0
*
*   Unless required by applicable law or agreed to in writing, software
*   distributed under the License is distributed on an "AS IS" BASIS,
*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*   See the License for the specific language governing permissions and
*   limitations under the License.
*/
require(dirname(__FILE__)."/DB.php");

class Sync {
	private $db;
        private $site_path = "/www/php5/";

        private $rsync = "/usr/bin/rsync";
        private $rsync_options;
                        //'-avz',
	/* Any additional rsync options like identity file. */
	private $rsync_ssh = '-e \'ssh -F /home/sync_user/.ssh/ssh_config\'';

        private $siteNeedles = array(
                "dev-",
                "dev."
        );

        private $rsync_source = "";
	/* Rsync target. user@hostname a'la ssh */
        private $rsync_target = "sync_user@example.com";

        private $currentSite;
        private $currentSource;
        private $currentTarget;
        private $siteList = array();

	private $is_loaded = false;

        // Exception Constants
        const ERROR_SITE_DOESNT_EXIST = "Sorry, the site you've chosen does not exist. Is it on this server?";

        public function __construct() {
		$this->db = DB::getInstance();
        }

        public function getSiteList() {
		$sth = $this->db->prepare("SELECT * FROM site_sync");
		$sth->execute();

		$this->siteList = $sth->fetchALL(PDO::FETCH_ASSOC);

		return $this->siteList;
        }

        public function pushDry($site_id) {
		// Load the site 
		$this->currentSite = $this->loadSite($site_id);

		$ret = array();
		$ret['del'] = array();
		$ret['add'] = array();

		$res = $this->psExec($this->genCMD('dry', array($this->currentSite['site_source'])));

		foreach($res as $file) {
			if(preg_match("#^\.\/$#", $file)) {
				continue;
			}

			// Temporary? Don't sync whole dirs?
			//if(preg_match("#.*\/$#", $file)) {
			//	continue;
			//}

			if(preg_match("#^deleting .*#", $file)) {
				$ret['del'][] = $file;
			} else {
				if(!preg_match("#\.$#", $file)) {
					$ret['add'][] = $file;
				}
			}
		}

		return $ret;
        }

	public function pushWet($site_id, $filelist = array()) {
		if(count($filelist) <= 0) {
			return;
		}

		$this->currentSite = $this->loadSite($site_id);

		return $this->psExec($this->genCMD('wet', $filelist));
	}

	public function loadSite($site_id) {
		$sth = $this->db->prepare("
			SELECT
				*
			FROM
				site_sync
			WHERE
				sync_id = ?
		");

		$sth->execute(array($site_id));
		$res = $sth->fetchAll(PDO::FETCH_ASSOC);

		// Make sure our paths have trailing slash
		if(!preg_match("#\/$#", $res[0]['site_source'])) {
			$res[0]['site_source'] .= "/";
		}

		if(!preg_match("#\/$#", $res[0]['site_target'])) {
			$res[0]['site_target'] .= "/";
		}

		$this->currentSite = $res[0];
		$this->is_loaded = true;

		return $res[0];
	}

	public function updateSyncTime($site_id = null) {
		if(!$this->is_loaded) {
			if($site_id != null) {
				$this->loadSite($site_id);
			}
		} else {
			$site_id = $this->currentSite['sync_id'];
		}

		$sth = $this->db->prepare("
			UPDATE 
				site_sync
			SET
				last_sync = now()
			WHERE
				sync_id = ?
		");
		$sth->execute(array($site_id));
	}

        private function setSource($site) {
                $this->currentSite = $site;
                $this->currentSource = $this->site_path.$this->currentSite;

                $this->currentTarget = $this->rsync_target;
        }

	private function psExec($cmd) {
		exec($cmd, $output);

		return $output;
	}

        private function genCMD($type = "dry", $filelist = array(), $delete = true) {
		if(count($filelist) <= 0) {
			return;
		}

		$flag = "";
		$format = "";
		switch(strtolower($type)) {
			case "dry":
				$flag = '-azn';
				$format = "--out-format=\"%n\"";

				break;
			case "wet":
				$flag = '-avzR';

				// Crease our paths for relative location sync
				$temp = array();
				foreach($filelist as $file) {
					$temp[] = $this->currentSite['site_source']."./".$file;
				}

				unset($filelist);
				$filelist = $temp;

				break;
			default:
				'-avzn';
		}

		

                $cmd = "";

		// Add apache to sudoers file if needed
                $cmd .= "sudo ";
                $cmd .= $this->rsync;
                $cmd .= " ";
		$cmd .= $format;
		$cmd .= " ";
                $cmd .= $flag;
		$cmd .= " ";

		if($delete) {
			$cmd .= "--delete";
                	$cmd .= " ";
		}

                $cmd .= $this->rsync_ssh;
                $cmd .= " ";
                $cmd .= $this->currentSite['site_excludes'];
		$cmd .= " ";
                $cmd .= implode(" ", $filelist);
		$cmd .= " ";
		$cmd .= $this->rsync_target.":".$this->currentSite['site_target'];

		//echo "<pre>"; print $cmd; die;

                return $cmd;
        }

        private function flipPush($flag) {
                $this->rsync_options = array_reverse($this->rsync_options);
                array_push($this->rsync_options, $flag);
                $this->rsync_options = array_reverse($this->rsync_options);
        }
}

?>
