#=
    Simple MCMC:
    Utilizes a Metropolis-Hastings Algorithm with a 
    standard Gibbs sampler.
=#

#=
    Basic linear equation function
=#

function lnfnc(x::Array{Float64}, a::Float64, b::Float64)
    y0 = Float64[]
    for xi in x
        y0i = (a * xi) + b
	push!(y0, y0i)
    end
    return y0
end

#=
    Basic chi squared function
    Note: data set error is variance not stddev
=#

function chisqr(yo::Array{Float64}, ye::Array{Float64}, syo::Array{Float64})
    csq = Float64[]
    for (yoi, yei, syoi) in zip(yo, ye, syo)
        csqi = (yoi - yei)^2 / syoi
        push!(csq, csqi)
    end
    return sum(csq)
end

#=
    Basic Metropolis-Hastings Algorithm with a 
    standard Gibbs sampler.
=#

function mcmc(a0, sa, b0, sb, x, y, dy)
    y0 = lnfnc(x, a0, b0)
    chi0 = chisqr(y0, y, dy)
    atrace = Float64[]
    btrace = Float64[]
    i = 0
    j = 0
    mcn = 500000
    brn = 1000
    while i < mcn
        at = (randn() * sa) + a0
        bt = (randn() * sb) + b0
        yt = lnfnc(x, at, bt)
        chit = chisqr(yt, y, dy)
        acal = e^((chi0 - chit) / 2.0)
        aexp = min(1.0, acal)
        u = rand()
        if u <= aexp
            a0 = at
            b0 = bt
            y0 = yt
            chi0 = chit
            push!(atrace, at)
            push!(btrace, bt)
            i += 1
        end
        j += 1
    end
end

#=
    Main
=#

x = Float64[203, 58, 210, 202,
            198, 158, 165, 201,
            157, 131, 166, 160,
            186, 125, 218, 146]

y = Float64[495, 173, 479, 504,
            510, 416, 393, 442,
            317, 311, 400, 337,
            423, 334, 533, 344]

dy = Float64[21, 15, 27, 14,
             30, 16, 14, 25,
             52, 16, 34, 31,
             42, 26, 16, 22]

a0 = 2.5
sa = 0.025
b0 = 28.82
sb = 2.5

# timing start
mcmc(a0, sa, b0, sb, x, y, dy)
# timing end
