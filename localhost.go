package sup

import (
	"fmt"
	"io"
	"os/exec"
	"os/user"
)

// Client is a wrapper over the SSH connection/sessions.
type LocalhostClient struct {
	cmd     *exec.Cmd
	user    string
	stdin   io.WriteCloser
	stdout  io.Reader
	stderr  io.Reader
	running bool
	env     string //export FOO="bar"; export BAR="baz";
}

func (c *LocalhostClient) Connect(_ string) error {
	u, err := user.Current()
	if err != nil {
		return err
	}

	c.user = u.Username
	return nil
}

func (c *LocalhostClient) Run(task *Task) error {
	var err error

	if c.running {
		return fmt.Errorf("Command already running")
	}

	cmd := exec.Command("bash", "-c", c.env+"set -x;"+task.Run)
	c.cmd = cmd

	c.stdout, err = cmd.StdoutPipe()
	if err != nil {
		return err
	}

	c.stderr, err = cmd.StderrPipe()
	if err != nil {
		return err
	}

	c.stdin, err = cmd.StdinPipe()
	if err != nil {
		return err
	}

	if err := c.cmd.Start(); err != nil {
		return ErrTask{task, err.Error()}
	}

	c.running = true
	return nil
}

func (c *LocalhostClient) Wait() error {
	if !c.running {
		return fmt.Errorf("Trying to wait on stopped command")
	}
	err := c.cmd.Wait()
	c.running = false
	return err
}

func (c *LocalhostClient) Close() error {
	return nil
}

func (c *LocalhostClient) Stdin() io.WriteCloser {
	return c.stdin
}

func (c *LocalhostClient) Stderr() io.Reader {
	return c.stderr
}

func (c *LocalhostClient) Stdout() io.Reader {
	return c.stdout
}

func (c *LocalhostClient) Prefix() (string, int) {
	host := c.user + "@localhost" + " | "
	return ResetColor + host, len(host)
}

func (c *LocalhostClient) Write(p []byte) (n int, err error) {
	return c.stdin.Write(p)
}

func (c *LocalhostClient) WriteClose() error {
	return c.stdin.Close()
}
