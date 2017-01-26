import math._
import scala.util.Random
import scala.collection.mutable.ArrayBuffer

object MCMC {

  def lnfnc(x: List[Double], a: Double, b: Double): List[Double] = x.map(xi => {(a * xi) + b})

  def chisqr(yo: List[Double], ye: List[Double], syo: List[Double]): Double = {
    (yo, ye, syo).zipped.map((yoi, yei, syoi) => {pow((yoi - yei), 2) / syoi}).sum
  }

  def mcmc(a0: Double, sa: Double, b0: Double, sb: Double, x: List[Double], y: List[Double], dy: List[Double]): (Int, Int, Double, Double) = {

    /**
      * Basic Metropolis-Hastings Algorithm with a
      * standard Gibbs sampler.
      *
      * Note: The scala in this method is pretty terrible...
      */

    val random = new Random
    val atrace = ArrayBuffer.empty[Double]
    val btrace = ArrayBuffer.empty[Double]
    val mcn = 500000
    val brn = 1000
    var aN = a0
    var bN = b0
    var y0 = lnfnc(x, a0, b0)
    var chi0 = chisqr(y0, y, dy)
    var i = 0
    var j = 0

    while (i < mcn) {
      var at = (random.nextGaussian * sa) + aN
      var bt = (random.nextGaussian * sb) + bN
      var yt = lnfnc(x, at, bt)
      var chit = chisqr(yt, y, dy)
      var acal = exp((chi0 - chit) / 2.0)
      var aexp = min(1.0, acal)
      var u = random.nextDouble

      if (u <= aexp) {
        i += 1
        atrace.append(at)
        btrace.append(bt)
        aN = at
        bN = bt
        y0 = yt
        chi0 = chit
      }
      j += 1

    }

    (i, j, atrace.drop(brn).sum/atrace.drop(brn).length.toDouble, btrace.drop(brn).sum/btrace.drop(brn).length.toDouble)
  }

  def main(args: Array[String]): Unit = {

    /**
      * Simple MCMC:
      * Utilizes a Metropolis-Hastings Algorithm with a
      * standard Gibbs sampler.
      */

    val X = List[Double](203, 58, 210, 202, 198, 158, 165, 201, 157, 131, 166, 160, 186, 125, 218, 146)
    val Y = List[Double](495, 173, 479, 504, 510, 416, 393, 442, 317, 311, 400, 337, 423, 334, 533, 344)
    val dY = List[Double](21, 15, 27, 14, 30, 16, 14, 25, 52, 16, 34, 31, 42, 26, 16, 22)

    val a0 = 2.5
    val sa = 0.025
    val b0 = 28.82
    val sb = 2.5

    val (apts, itts, parama, paramb) = mcmc(a0, sa, b0, sb, X, Y, dY)
    //println(apts)
    //println(itts)
    //println(parama)
    //println(paramb)

  }

}
