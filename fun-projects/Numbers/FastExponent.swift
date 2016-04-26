func fast_exponent(value value :Int, topower exp :Int) -> Int {
    guard exp >= 0 else {
        return -1
    }
    if exp == 0 {
        return 1
    }

    if value == 2 {
        return 1 << exp
    }

    if value == 1 {
        return value
    }

    var p2: Int = 1, power: Int = value

    while (p2 << 1) <= exp {
        power *= power
        p2 <<= 1
    }

    if p2 < exp {
        power *= fast_exponent(value: value, topower: exp - p2)
    }
    return power
}


print("Enter two numbers a and b seperated by space: ", terminator:"")
let inputs = readLine()!.characters.split(){$0==" "}.map{Int(String($0))}

print(fast_exponent(value: inputs[0]!, topower:inputs[1]!))