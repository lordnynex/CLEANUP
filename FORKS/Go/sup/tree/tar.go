package sup

import (
	"fmt"
	"io"
	"strings"

	"os/exec"
)

// Copying dirs/files over SSH using TAR.
// tar -C . -cvzf - <dirs/files> | ssh <host> "tar -C <dst_dir> -xvzf -"

// RemoteTarCommand returns command to be run on remote SSH host
// to properly receive the created TAR stream.
// TODO: Check for relative directory.
func RemoteTarCommand(dir string) string {
	return fmt.Sprintf("tar -C \"%s\" -xzf -", dir)
}

func LocalTarCommand(path, exclude string) string {

	// Added pattens to exclude from tar compress
	excludes := ""

	result := strings.Split(exclude, ",")

	for _, exclude := range result {
		excludes += `--exclude=` + strings.TrimSpace(exclude) + ` `
	}

	return fmt.Sprintf("tar %s -C '.' -czf - %s", excludes, path)
}

// NewTarStreamReader creates a tar stream reader from a local path.
// TODO: Refactor. Use "archive/tar" instead.
func NewTarStreamReader(path, exclude, env string) io.Reader {
	cmd := exec.Command("bash", "-c", env+LocalTarCommand(path, exclude))

	stdout, err := cmd.StdoutPipe()
	if err != nil {
		return nil
	}

	stderr, err := cmd.StderrPipe()
	if err != nil {
		return nil
	}

	output := io.MultiReader(stdout, stderr)

	if err := cmd.Start(); err != nil {
		return nil
	}

	return output
}
