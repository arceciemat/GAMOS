def GetMarker( imar ) :

    imars = imar%10 
    if imars == 0 :
        lmark = 'o'
    elif imars == 2 :
        lmark = '*'
    elif imars == 1 :
        lmark = 'x'
    elif imars == 3 :
        lmark = '^'
    elif imars == 4 :
        lmark = 's'
    elif imars == 5 :
        lmark = 'd'
    elif imars == 6 :
        lmark = 'v'
    elif imars == 7 :
        lmark = '.'
    elif imars == 8 :
        lmark = '<'
    elif imars == 9 :
        lmark = '>'

    return lmark

