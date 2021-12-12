package main

import (
    "fmt"
    "os"
    
    "github.com/iancoleman/strcase"
)

const (
    headerFormatString = `#pragma once

#ifndef %s_H
#define %s_H

class %s {

};

#endif // %s_H`
    sourceFormatString = `#include "%s"`
)

func create(path string, fname string) (*os.File, error) {
    if err := os.MkdirAll(path, 0770); err != nil {
        return nil, err
    }
    return os.Create(path + "/" + fname)
}

func writeHeaderDefaults(path string, fname string) error {
    capitalisedFName := strcase.ToScreamingSnake(fname)
    formattedHeader := fmt.Sprintf(
        headerFormatString,
        capitalisedFName,
        capitalisedFName,
        strcase.ToCamel(fname),
        capitalisedFName,
    )
    return os.WriteFile(path, []byte(formattedHeader), 0644)
}

func writeSourceDefaults(path string, fname string) error {
    formattedSource := fmt.Sprintf(
        sourceFormatString,
        fname,
    )
    return os.WriteFile(path, []byte(formattedSource), 0644)
}

func main() {
    create(os.Args[1], os.Args[2] + ".cpp")
    create(os.Args[1], os.Args[2] + ".hpp")
    writeHeaderDefaults(os.Args[1] + "/" + os.Args[2] + ".hpp", os.Args[2])
    writeSourceDefaults(os.Args[1] + "/" + os.Args[2] + ".cpp", os.Args[2] + ".hpp")
}