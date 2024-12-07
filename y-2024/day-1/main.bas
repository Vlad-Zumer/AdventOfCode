' DEVELOPED WITH AND FOR FREE BASIC
'
' fbc64 -x ./out/main.exe main.bas && ./out/main

sub qSortEx(array() as integer, lb as integer, ub as integer)

    ' print ""
    ' print "lb";lb
    ' print "ub";ub
    ' print ""

    if (ub < lb) or (((ub - lb) + 1) <= 1) then
        return
    end if
    
    dim pivotPos as integer
    dim pivotVal as integer
    pivotPos = lb + (((ub - lb) + 1) \ 2)
    pivotVal = array(pivotPos)

    dim lowP as integer
    dim highP as integer
    lowP = lb
    highP = ub

    ' [A,A,A,A,A,A,A,A,P,B,B,B,B,B,B,B,B]
    ' A <= P
    ' B > P

    while (lowP < pivotPos) and (highP > pivotPos)
        if (array(lowP) > pivotVal) and (array(highP) <= pivotVal) then
            swap array(lowP), array(highP)
            lowP = lowP + 1
            highP = highP - 1
        elseif array(lowP) <= pivotVal then
            lowP = lowP +1
        elseif array(highP) > pivotVal then
            highP = highP - 1
        end if
    wend

    while lowP < pivotPos
        if array(lowP) > pivotVal then
            swap array(lowP), array(pivotPos)
            pivotPos = pivotPos - 1
            swap array(lowP), array(pivotPos)
        else
            lowP = lowP + 1
        end if
    wend
    
    while (highP > pivotPos)
        if array(highP) <= pivotVal then
            swap array(highP), array(pivotPos)
            pivotPos = pivotPos + 1
            swap array(highP), array(pivotPos)
        else
            highP = highP - 1
        end if
    wend
    
    ' print "----- (lb, pp-1)"
    ' print "lb";lb
    ' print "pp";pivotPos
    ' print "ub";ub
    ' print "-----"
    qSortEx(array(), lb, pivotPos-1)

    ' print "----- (pp+1, ub)"
    ' print "lb";lb
    ' print "pp";pivotPos
    ' print "ub";ub
    ' print "-----"
    qSortEx(array(), pivotPos+1, ub)
    
end sub

sub qSort(array() as integer)
    dim lb as integer
    dim ub as integer
    
    lb = lbound(array)
    ub = ubound(array)

    qSortEx(array(), lb, ub)
end sub

sub pushVal( array() as integer, value as integer)
    dim lb as integer
    dim ub as integer
    lb = lbound(array)
    ub = ubound(array)
    redim preserve array(lb to ub + 1)
    array(ub+1) = value
end sub

sub printArray( array() as integer )
    for i as integer = lbound(array) to ubound(array)
        print Str(array(i));" ";
    next
    print
end sub

sub parseInput(fileName as string, l1() as integer, l2() as integer)
    open fileName for input as #1

    dim l1v as integer
    dim l2v as integer
    do until( eof(1) )
	    input #1, l1v, l2v
        pushVal(l1(), l1v)
        pushVal(l2(), l2v)
    loop

    close #1
end sub

function p1( a1() as integer, a2() as integer) as integer
    dim res() as integer
    for i as integer = lbound(a1) to ubound(a1)
        pushVal(res(), abs(a1(i) - a2(i)))
    next
    dim s as integer
    s = 0
    for i as integer = lbound(res) to ubound(res)
        s = s + res(i)
    next
    return s
end function

function p2( a1() as integer, a2() as integer) as integer
    dim sum as integer
    dim mult as integer
    dim j as integer
    sum = 0
    for i as integer = lbound(a1) to ubound(a1)
        j = lbound(a2)
        mult = 0
        while a2(j) <= a1(i)
            if a2(j) = a1(i) then
                mult = mult + 1
            end if
            j = j + 1
        wend
        sum = sum + (a1(i) * mult)
    next
    return sum
end function


dim l1() as integer
dim l2() as integer

' parseInput("test.txt", l1(), l2())
parseInput("input.txt", l1(), l2())

qSort(l1())
qSort(l2())

print "p1=";p1(l1(), l2())
print "p2=";p2(l1(), l2())

erase l1
erase l2

print "DONE!"
