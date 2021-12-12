package main

import (
    "fmt"
    "os"
    "strings"
    "path/filepath"
)

func pathHandler(path string, info os.FileInfo, err error) error {
    if err != nil {
        return err
    }
    if strings.HasSuffix(path, ".h") {
        os.Rename(path, path + "pp")
        fmt.Println(path)
    }
    return nil
}

func main() {
	err := filepath.Walk(
        ".",
        pathHandler,
    )
    if err != nil {
        fmt.Println(err)
    }
}