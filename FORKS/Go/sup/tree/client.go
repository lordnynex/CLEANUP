package sup

import "io"

type Client interface {
	Connect(host string) error
	Run(task *Task) error
	Wait() error
	Close() error
	Prefix() (string, int)
	Write(p []byte) (n int, err error)
	WriteClose() error
	Stdin() io.WriteCloser
	Stderr() io.Reader
	Stdout() io.Reader
}
