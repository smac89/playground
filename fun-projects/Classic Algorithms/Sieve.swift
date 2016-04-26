import Foundation
#if os(Linux)
import Glibc
#endif

/**
 Given a list of booleans all initialized to true, will sieve the list
 and set all the values to false which are not primes
 
 - Parameter primeNet: A reference to a list of booleans all initialized to true
 */

public func sieveOfEratosthenes(inout primeNet : [Bool]) {
    for i in 0..<primeNet.count {
        primeNet[i] = true
    }

    // explicitly set 1 to false
    primeNet[0] = false; primeNet[1] = false

    for p in 2...Int(sqrt(Double(primeNet.count)) + 1) {
        if primeNet[p] {
            for j in (p * p).stride(to: primeNet.count, by: p) {
                primeNet[j] = false
            }
        }
    }
}

let mcount = 10000001
var mseive = Array<Bool>(count: mcount, repeatedValue: true)

let start = NSDate()
sieveOfEratosthenes(&mseive)
let end : Double = NSDate().timeIntervalSinceDate(start)

var primes: Int = 0
for i in 1..<mcount {
    if mseive[i] {
        primes += 1
    }
}

print ("There are \(primes) below \(mcount). This took \(end) seconds")