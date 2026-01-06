#!/usr/bin/env bash
set -e

BIN=./bin/qmac
TMP=$(mktemp)

pass() {
    printf "%-36s ....... \\[^_^]/\n" "$1"
}

fail() {
    echo
    echo "[FAIL] $1"
    echo "Expected:"
    echo "$2"
    echo
    echo "Got:"
    cat "$TMP"
    rm -f "$TMP"
    exit 1
}

run_test() {
    name="$1"
    cmd="$2"
    expected="$3"

    eval "$BIN $cmd" > "$TMP"

    normalize() {
        sed -E 's/[[:space:]]+/ /g; s/ *\| */ | /g; s/ $//'
    }

    if diff -u \
       <(printf "%s\n" "$expected" | normalize) \
       <(cat "$TMP" | normalize) >/dev/null; then
        pass "$name"
    else
        fail "$name" "$expected"
    fi
}

# --------------------------------------------------
# 1. Single essential implicant
# --------------------------------------------------
run_test \
"v3 basic essential" \
"-v 3 -m 1,3,5,7" \
"<term: 1 | mask: 6 | ones_count: 1 | combined: 0 | is_dontcare: 0>"

# --------------------------------------------------
# 2. Block of minterms (single cube)
# --------------------------------------------------
run_test \
"v4 contiguous block" \
"-v 4 -m 4,5,6,7" \
"<term: 4 | mask: 3 | ones_count: 1 | combined: 0 | is_dontcare: 0>"

# --------------------------------------------------
# 3. Cyclic dominance (no initial essentials)
# Classic QM trap case
# --------------------------------------------------
run_test \
"v4 cyclic dominance" \
"-v 4 -m 1,3,7,11,15" \
"<term: 1 | mask: 2 | ones_count: 1 | combined: 0 | is_dontcare: 0>
<term: 3 | mask: 12 | ones_count: 2 | combined: 0 | is_dontcare: 0>"

# --------------------------------------------------
# 4. Mixed essentials + dominance
# --------------------------------------------------
run_test \
"v4 mixed reduction" \
"-v 4 -m 0,1,2,5,6,7,8,9,10,14,15" \
"<term: 1 | mask: 4 | ones_count: 1 | combined: 0 | is_dontcare: 0>
<term: 0 | mask: 9 | ones_count: 0 | combined: 0 | is_dontcare: 0>
<term: 0 | mask: 10 | ones_count: 0 | combined: 0 | is_dontcare: 0>
<term: 6 | mask: 9 | ones_count: 2 | combined: 0 | is_dontcare: 0>"

# --------------------------------------------------
# 5. Don't-care driven simplification
# --------------------------------------------------
run_test \
"v3 with dontcares" \
"-v 3 -m 1,3,7 -d 0,2,5" \
"<term: 1 | mask: 6 | ones_count: 1 | combined: 0 | is_dontcare: 0>"

# --------------------------------------------------
# 6. Large stress (12 vars, structured)
# --------------------------------------------------
run_test \
"v12 stress structured" \
"-v 12 -m 0,1,2,3,4,5,6,7,16,17,18,19,20,21,22,23,32,33,34,35,36,37,38,39" \
"<term: 0 | mask: 23 | ones_count: 0 | combined: 0 | is_dontcare: 0>
<term: 0 | mask: 39 | ones_count: 0 | combined: 0 | is_dontcare: 0>"

# --------------------------------------------------
# 7. Extreme density (stress + reduction)
# --------------------------------------------------
run_test \
"v10 dense stress" \
"-v 10 -m $(seq -s, 0 511)" \
"<term: 0 | mask: 511 | ones_count: 0 | combined: 0 | is_dontcare: 0>"

rm -f "$TMP"

echo
pass "prime implicant tests passed"
