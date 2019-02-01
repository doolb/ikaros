#!/bin/bash
export LC_ALL=C LANG=C
for file in "$@" ; do
    readelf -s "$file" | while read num value size type bind vix index name dummy ; do
        [ "$type" = "FUNC" ] || continue
        [ "$bind" = "GLOBAL" ] || continue
 #       [ "$num"] = "$[$num]"] || continue
        [ "$index" = "$[$index]" ] || continue
        case "$name" in
            test*) printf '%s %s\n' "$file" "$name"
                    ;;
        esac
    done
done
