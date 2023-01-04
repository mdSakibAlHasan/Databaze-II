def apriori(data, min_support,length):
<<<<<<< HEAD
    # First, generate a list of all unique items in the data
=======
    # gather data in a set
>>>>>>> 8259717f0389362d2bb21384a9a21c21a3726c7f
    items = set()
    for transaction in data:
        for item in transaction:
            items.add(item)

    # Next, generate a list of all item sets of length 1
    item_sets = []
    for item in items:
        item_sets.append(frozenset([item]))

<<<<<<< HEAD
    # Initialize a dictionary to store the support counts for each item set
    support_counts = {}
    frequent_item_sets = []

    # Iterate through each item set and count the support for each one
=======
    
    support_counts = {}
    frequent_item_sets = []

    # support count of each set
>>>>>>> 8259717f0389362d2bb21384a9a21c21a3726c7f
    for item_set in item_sets:
        support_counts[item_set] = 0
        for transaction in data:
            if item_set.issubset(transaction):
                support_counts[item_set] += 1

    if length == 1:
        return support_counts, frequent_item_sets
    
    # Remove item sets with support less than the minimum support threshold
    
    for item_set, count in support_counts.items():
        if count >= min_support:
            frequent_item_sets.append(item_set)

    # Continue the process for item sets of length 2 and up
    k = 2
    while len(frequent_item_sets) > 0:
        # Generate a list of all item sets of length k
        item_sets = []
        for item_set_1 in frequent_item_sets:
            for item_set_2 in frequent_item_sets:
                if item_set_1 != item_set_2:
                    item_set = item_set_1.union(item_set_2)
                    if len(item_set) == k:
                        item_sets.append(item_set)

        # Initialize a dictionary to store the support counts for each item set
        support_counts = {}

        # Iterate through each item set and count the support for each one
        for item_set in item_sets:
            support_counts[item_set] = 0
            for transaction in data:
                if item_set.issubset(transaction):
                    support_counts[item_set] += 1

        # Remove item sets with support less than the minimum support threshold
        frequent_item_sets = []
        for item_set, count in support_counts.items():
            if count >= min_support:
                frequent_item_sets.append(item_set)

        
        if k == length:
            return support_counts,frequent_item_sets
        
        k += 1

    return support_counts , frequent_item_sets


book_data = [
    ['I1', 'I2', 'I5'],
    ['I2', 'I4'],
    [ 'I2', 'I3'],
    [ 'I1', 'I2', 'I4'],
    ['I1', 'I3'],
    [ 'I2', 'I3'],
    [ 'I1', 'I3'],
    ['I1', 'I2', 'I3', 'I5'],
    ['I1', 'I2', 'I3']
]


def find_combination():

    # data = [
    #     ['bread', 'milk'],
    #     ['bread', 'diaper', 'beer', 'eggs'],
    #     ['milk', 'diaper', 'beer', 'cola'],
    #     ['bread', 'milk', 'diaper', 'beer'],
    #     ['bread', 'milk', 'diaper', 'cola']
    # ]

    min_support = 2
<<<<<<< HEAD
    length = 3
=======
    length = 2
>>>>>>> 8259717f0389362d2bb21384a9a21c21a3726c7f


    support_counts, frequent_item_set = apriori(book_data, min_support,length)
    if not frequent_item_set:
        print("No combination found")

    for keys,count in support_counts.items():
        if count >= min_support:
            print(keys," - ",count)

find_combination()


def find_confidence(data, min_support, str):
    str = str.replace(" ","")
    str = str.split("=")
    denominator , nominator = str  
    denominator = denominator.split("^")
    nominator = nominator.split("^")
    
    denominator_set = frozenset(denominator)
    for value in denominator:
        nominator.append(value)
    nominator_set = frozenset(nominator)
    
  
    nominator_count , denominator_count=0,0
    support_count,frequent_item_set= apriori(book_data, min_support,length =len(denominator))  
    for item in support_count:
        if item.issubset(denominator_set):
            denominator_count = support_count[item]
    
    support_count,frequent_item_set= apriori(book_data, min_support,length =len(nominator))
    for item in support_count:
        if item.issubset(nominator_set):
            nominator_count = support_count[item]
            
    try:
        print("Confidence level: ",nominator_count/denominator_count)
    except ZeroDivisionError:
        print("Confidence level not found because of zero division" )



    

strs = "I2 = I1 ^ I5"
strsp = "I1 = I2 ^ I5"
<<<<<<< HEAD
find_confidence(book_data,min_support=2,str= strsp)
=======
# find_confidence(book_data,min_support=2,str= strsp)
>>>>>>> 8259717f0389362d2bb21384a9a21c21a3726c7f
