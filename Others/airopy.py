def apriori(data, min_support, length):
   
    items = set()
    for transaction in data:
        for item in transaction:
            items.add(item)


    item_sets = []
    for item in items:
        item_sets.append(frozenset([item]))

   
    support_counts = {}
    frequent_item_sets = []


    for item_set in item_sets:
        support_counts[item_set] = 0
        for transaction in data:
            if item_set.issubset(transaction):
                support_counts[item_set] += 1

    if length == 1:
        return support_counts, frequent_item_sets

   

    for item_set, count in support_counts.items():
        if count >= min_support:
            frequent_item_sets.append(item_set)

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

       
        support_counts = {}

        
        for item_set in item_sets:
            support_counts[item_set] = 0
            for transaction in data:
                if item_set.issubset(transaction):
                    support_counts[item_set] += 1

        frequent_item_sets = []
        for item_set, count in support_counts.items():
            if count >= min_support:
                frequent_item_sets.append(item_set)
               
        if k == length:
            return support_counts, frequent_item_sets

        k += 1

    return support_counts, frequent_item_sets


book_data = [
    ['I1', 'I2', 'I5'],
    ['I2', 'I4'],
    ['I2', 'I3'],
    ['I1', 'I2', 'I4'],
    ['I1', 'I3'],
    ['I2', 'I3'],
    ['I1', 'I3'],
    ['I1', 'I2', 'I3', 'I5'],
    ['I1', 'I2', 'I3']
]

transactions = [
    ["MILK", "BREAD", "BISCUIT"],
    ["BREAD", "MILK", "BISCUIT", "CORNFLAKES"],
    ["BREAD", "TEA", "BOURNVITA"],
    ["JAM", "MAGGI", "BREAD", "MILK"],
    ["MAGGI", "TEA", "BISCUIT"],
    ["BREAD", "TEA", "BOURNVITA"],
    ["MAGGI", "TEA", "CORNFLAKES"],
    ["MAGGI", "BREAD", "TEA", "BISCUIT"],
    ["JAM", "MAGGI", "BREAD", "TEA"],
    ["BREAD", "MILK"],
    ["COFFEE", "COCK", "BISCUIT", "CORNFLAKES"],
    ["COFFEE", "COCK", "BISCUIT", "CORNFLAKES"],
    ["COFFEE", "SUGER", "BOURNVITA"],
    ["BREAD", "COFFEE", "COCK"],
    ["BREAD", "SUGER", "BISCUIT"],
    ["COFFEE", "SUGER", "CORNFLAKES"],
    ["BREAD", "SUGER", "BOURNVITA"],
    ["BREAD", "COFFEE", "SUGER"],
    ["BREAD", "COFFEE", "SUGER"],
    ["TEA", "MILK", "COFFEE", "CORNFLAKES"]
]


data = [
    ['bread', 'milk'],
    ['bread', 'diaper', 'beer', 'eggs'],
    ['milk', 'diaper', 'beer', 'cola'],
    ['bread', 'milk', 'diaper', 'beer'],
    ['bread', 'milk', 'diaper', 'cola']
]


def find_combination():

    min_support = 2
    # length = int(input("Input your length: "))
    length = 3

    support_counts, frequent_item_set = apriori(
        transactions, min_support, length)
    if not frequent_item_set:
        print("No combination found")

    for keys, count in support_counts.items():
        if count >= min_support:
            print(keys, " - ", count)


find_combination()


def find_confidence(data, min_support, str):
    str = str.replace(" ", "")
    str = str.split("=")
    denominator, nominator = str
    denominator = denominator.split("^")
    nominator = nominator.split("^")

    denominator_set = frozenset(denominator)
    for value in denominator:
        nominator.append(value)
    nominator_set = frozenset(nominator)

    nominator_count, denominator_count = 0, 0
    support_count, frequent_item_set = apriori(
        book_data, min_support, length=len(denominator))
    for item in support_count:
        if item.issubset(denominator_set):
            denominator_count = support_count[item]

    support_count, frequent_item_set = apriori(
        book_data, min_support, length=len(nominator))
    for item in support_count:
        if item.issubset(nominator_set):
            nominator_count = support_count[item]

    try:
        print("Confidence level: ", nominator_count/denominator_count)
    except ZeroDivisionError:
        print("Confidence level not found because of zero division")


# strs = "I2 = I1 ^ I5"
# strsp = "I1 = I2 ^ I5"
# find_confidence(book_data,min_support=2,str= strsp)
