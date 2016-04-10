package commands

import (
	"flag"
	"fmt"
	"os"

	"github.com/google/subcommands"
	c "github.com/kotakanbe/go-cve-dictionary/config"
	"github.com/kotakanbe/go-cve-dictionary/db"
	log "github.com/kotakanbe/go-cve-dictionary/log"
	"github.com/kotakanbe/go-cve-dictionary/nvd"
	server "github.com/kotakanbe/go-cve-dictionary/server"
	"golang.org/x/net/context"
)

// ServerCmd is Subcommand for CVE dictionary HTTP Server
type ServerCmd struct {
	debug    bool
	debugSQL bool

	dbpath string
	bind   string
	port   string
}

// Name return subcommand name
func (*ServerCmd) Name() string { return "server" }

// Synopsis return synopsis
func (*ServerCmd) Synopsis() string { return "Start CVE dictionary HTTP server" }

// Usage return usage
func (*ServerCmd) Usage() string {
	return `server:
	server
		[-bind=127.0.0.1]
		[-port=8000]
		[-dpath=$PWD/cve.sqlite3]
		[-debug]
		[-debug-sql]

`
}

// SetFlags set flag
func (p *ServerCmd) SetFlags(f *flag.FlagSet) {
	f.BoolVar(&p.debug, "debug", false,
		"debug mode (default: false)")
	f.BoolVar(&p.debugSQL, "debug-sql", false,
		"SQL debug mode (default: false)")

	pwd := os.Getenv("PWD")
	f.StringVar(&p.dbpath, "dbpath", pwd+"/cve.sqlite3",
		fmt.Sprintf("/path/to/sqlite3 (default : %s)", pwd+"/cve.sqlite3"))

	f.StringVar(&p.bind,
		"bind",
		"127.0.0.1",
		"HTTP server bind to IP address (default: loop back interface)")
	f.StringVar(&p.port, "port", "1323",
		"HTTP server port number (default: 1323)")
}

// Execute execute
func (p *ServerCmd) Execute(_ context.Context, f *flag.FlagSet, _ ...interface{}) subcommands.ExitStatus {

	c.Conf.Debug = p.debug
	c.Conf.DebugSQL = p.debugSQL

	if c.Conf.Debug {
		log.SetDebug()
	}

	c.Conf.Bind = p.bind
	c.Conf.Port = p.port
	c.Conf.DBPath = p.dbpath

	if !c.Conf.Validate() {
		return subcommands.ExitUsageError
	}

	log.Infof("Opening DB. datafile: %s", c.Conf.DBPath)
	if err := db.OpenDB(); err != nil {
		log.Error(err)
		return subcommands.ExitFailure
	}

	log.Info("Migrating DB")
	if err := db.MigrateDB(); err != nil {
		log.Error(err)
		return subcommands.ExitFailure
	}

	count, err := db.CountNvd()
	if err != nil {
		log.Errorf("Failed to count NVD table: %s", err)
		return subcommands.ExitFailure
	}

	if count == 0 {
		log.Info("Fetching vulnerability data from NVD because no NVD data found in DB.")
		entries, err := nvd.FetchFiles()
		if err != nil {
			log.Error(err)
			return subcommands.ExitFailure
		}

		if err := db.InsertNvd(entries); err != nil {
			log.Errorf("Failed to insert. dbpath: %s, err: %s",
				c.Conf.DBPath, err)
			return subcommands.ExitFailure
		}

		// Exit because fetching NVD data costs huge memory and keeping...
		log.Info("Success")
		return subcommands.ExitSuccess
	}

	log.Info("Starting HTTP Sever...")
	if err := server.Start(); err != nil {
		log.Error(err)
		return subcommands.ExitFailure
	}
	return subcommands.ExitSuccess
}
