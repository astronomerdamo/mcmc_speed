/*

    Simple MCMC:
    Utilizes a Metropolis-Hastings Algorithm with a 
    standard Gibbs sampler.

*/

package main

import (
        "fmt"
        "math"
        "math/rand"
        "time"
)

func lnfnc(x [16]int, a float64, b float64) [16]float64 {
/*
    Basic linear equation function
*/
    var y0 [16]float64
    for i := 0; i < len(x); i++ {
        y0[i] = a * float64(x[i]) + b
    }
    return y0
}

func chisqr(yo [16]float64, ye [16]int, syo [16]int) float64 {
/*
    Basic chi squared function
    Note: data set error is variance not stddev
*/
    csq := 0.0
    for i := 0; i < len(ye); i++ {
        csq += math.Pow(yo[i] - float64(ye[i]), 2) / float64(syo[i])
    }
    return csq
}

func mcmc(a0 float64, sa float64, b0 float64, sb float64,
          x [16]int, y [16]int, dy [16]int) {
/*
    Basic Metropolis-Hastings Algorithm with a 
    standard Gibbs sampler.
*/
    const (
           mcn = 500000
           brn = 1000
    )
    var (
         i = 0
         j = 0
    )

    y0 := lnfnc(x, a0, b0)
    chi0 := chisqr(y0, y, dy)

    var atrace [mcn]float64
    var btrace [mcn]float64

    for {
        at := rand.NormFloat64() * sa + a0
        bt := rand.NormFloat64() * sb + b0

        yt := lnfnc(x, at, bt)
        chit := chisqr(yt, y, dy)

        acal := math.Exp((chi0 - chit) / 2.0)
        aexp := math.Min(1.0, acal)
        u := rand.Float64()

        if u <= aexp {
            a0 = at
            b0 = bt
            y0 = yt
            chi0 = chit
            atrace[i] = at
            btrace[i] = bt
            i += 1
        }
        j += 1

        if i == mcn {
            fmt.Println("i: ", i, " j: ", j)
            fmt.Println("Acceptance ratio:", float64(i)/float64(j))

            aave := 0.0
            bave := 0.0
            for k := brn; k < mcn; k++ {
                aave += atrace[k]
                bave += btrace[k]
            }

            aave = aave / float64(mcn - brn)
            bave = bave / float64(mcn - brn)
            fmt.Println("a: ", aave, " b: ", bave)

            break
        }
    }
}

func main() {
    
    x := [16]int{203,  58, 210, 202,
                 198, 158, 165, 201,
                 157, 131, 166, 160,
                 186, 125, 218, 146}

    y := [16]int{495, 173, 479, 504,
                 510, 416, 393, 442,
                 317, 311, 400, 337,
                 423, 334, 533, 344}

    dy := [16]int{21, 15, 27, 14,
                  30, 16, 14, 25,
                  52, 16, 34, 31,
                  42, 26, 16, 22}

    a0 := 2.5
    sa := 0.025
    b0 := 28.82
    sb := 2.5

    cpubeg := time.Now()
    rand.Seed(cpubeg.UnixNano())    
    mcmc(a0, sa, b0, sb, x, y, dy)
    cpuend := time.Now()
    fmt.Println("CPU time: ", cpuend.Sub(cpubeg))
}
