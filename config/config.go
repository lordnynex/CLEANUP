package config

import (
	valid "github.com/asaskevich/govalidator"
	log "github.com/kotakanbe/go-cve-dictionary/log"
)

// Conf has Configuration
var Conf Config

// Config has config
type Config struct {
	Debug    bool
	DebugSQL bool

	DumpPath string
	DBPath   string

	FetchJvnWeek       bool
	FetchJvnMonth      bool
	FetchJvnEntire     bool
	FetchJvnPeriodChar string

	FetchNvdLast2Y bool

	Bind string `valid:"ipv4"`
	Port string `valid:"port"`

	//TODO validate
	HTTPProxy string
}

// Validate validates configuration
func (c *Config) Validate() bool {
	if ok, _ := valid.IsFilePath(c.DBPath); !ok {
		log.Errorf("SQLite3 DB path must be a *Absolute* file path. dbpath: %s", c.DBPath)
		return false
	}

	if len(c.DumpPath) != 0 {
		if ok, _ := valid.IsFilePath(c.DumpPath); !ok {
			log.Errorf("JSON path must be a *Absolute* file path. dumppath: %s", c.DumpPath)
			return false
		}
	}

	_, err := valid.ValidateStruct(c)
	if err != nil {
		log.Errorf("error: " + err.Error())
		return false
	}
	return true
}
