func reverseString() -> String {
    // get a string from the user
    let str : String = readLine() ?? ""
    var rev :String = ""

    for c in str.characters.reverse() {
        rev.append(c)
    }
    return rev
}

print (reverseString())