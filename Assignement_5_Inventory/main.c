#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 

#define MAX_NAME_LENGTH 50 

typedef struct {
    int id;
    char name[MAX_NAME_LENGTH + 1]; 
    float price;
    int quantity;
} Product;

void displayMenu();
Product getProductDetails();
void addProduct(Product **inventory, int *numProducts);
void viewAllProducts(Product *inventory, int numProducts);
int findProductIndex(Product *inventory, int numProducts, int targetId);
void updateQuantity(Product *inventory, int numProducts);
void searchById(Product *inventory, int numProducts);
void searchByName(Product *inventory, int numProducts);
void searchByPriceRange(Product *inventory, int numProducts);
void deleteProduct(Product **inventory, int *numProducts);

int main() {
    Product *inventory = NULL;
    int numProducts = 0;
    int initialCount = 0;
    int choice = 0;

    printf("Enter initial number of products: ");
    scanf(" %d", &initialCount); 

    if (initialCount < 0 || initialCount > 100) {
        printf("Invalid initial count. Must be between 0 and 100.\n");
        return 1;
    }

    if (initialCount > 0) {
        inventory = (Product *)calloc(initialCount, sizeof(Product));

        if (inventory == NULL) {
            printf("Error: Initial memory allocation failed.\n");
            return 1; 
        }

        numProducts = initialCount;

        for (int i = 0; i < numProducts; i++) {
            printf("\nEnter details for product %d:\n", i + 1);
            Product newProduct = getProductDetails();

            int duplicateIndex = findProductIndex(inventory, i, newProduct.id); 
            
            if (duplicateIndex != -1) {
                printf("Error: Product ID %d already exists. Please try again.\n", newProduct.id);
                i--; 
            } else {
                inventory[i] = newProduct; 
            }
        }
    }

    while (1) {
        displayMenu();
        if (scanf(" %d", &choice) != 1) { 
            printf("Invalid input. Please enter a number.\n");
            
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
            case 1:
                addProduct(&inventory, &numProducts);
                break;
            case 2:
                viewAllProducts(inventory, numProducts);
                break;
            case 3:
                updateQuantity(inventory, numProducts);
                break;
            case 4:
                searchById(inventory, numProducts);
                break;
            case 5:
                searchByName(inventory, numProducts);
                break;
            case 6:
                searchByPriceRange(inventory, numProducts);
                break;
            case 7:
                deleteProduct(&inventory, &numProducts);
                break;
            case 8:
                free(inventory);
                printf("\nMemory released successfully. Exiting program...\n");
                exit(0);
            default:
                printf("Invalid choice. Please enter a number between 1 and 8.\n");
        }
    }

    return 0;
}

void displayMenu() {
    printf("\n========= INVENTORY MENU =========\n");
    printf("1. Add New Product\n");
    printf("2. View All Products\n");
    printf("3. Update Quantity\n");
    printf("4. Search Product by ID\n");
    printf("5. Search Product by Name\n");
    printf("6. Search Product by Price Range\n");
    printf("7. Delete Product\n");
    printf("8. Exit\n");
    printf("Enter your choice: ");
}

Product getProductDetails() {
    Product p;
    printf("Product ID: ");
    scanf(" %d", &p.id); 

    printf("Product Name: ");
    scanf(" %50[^\n]", p.name); 

    printf("Product Price: ");
    scanf(" %f", &p.price); 

    printf("Product Quantity: ");
    scanf(" %d", &p.quantity); 

    return p;
}

void addProduct(Product **inventory, int *numProducts) {
    printf("\nEnter new product details:\n");
    Product newProduct = getProductDetails();

    int duplicateIndex = findProductIndex(*inventory, *numProducts, newProduct.id);
    
    if (duplicateIndex != -1) {
        printf("Error: Product ID %d already exists. Product not added.\n", newProduct.id);
        return; 
    }

    int newCount = *numProducts + 1;
    Product *temp = (Product *)realloc(*inventory, newCount * sizeof(Product));

    if (temp == NULL) {
        printf("Error: Memory reallocation failed. Product not added.\n");
        return; 
    }
    
    *inventory = temp;
    (*inventory)[*numProducts] = newProduct; 
    *numProducts = newCount;

    printf("Product added successfully!\n");
}

void viewAllProducts(Product *inventory, int numProducts) {
    printf("\n========= PRODUCT LIST =========\n");
    if (numProducts == 0) {
        printf("Inventory is empty.\n");
        return;
    }

    for (int i = 0; i < numProducts; i++) {
        printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",
               inventory[i].id,
               inventory[i].name,
               inventory[i].price,
               inventory[i].quantity);
    }
}

int findProductIndex(Product *inventory, int numProducts, int targetId) {
    for (int i = 0; i < numProducts; i++) {
        if (inventory[i].id == targetId) {
            return i;
        }
    }
    return -1; 
}

void updateQuantity(Product *inventory, int numProducts) {
    int targetId, newQuantity;
    printf("\nEnter Product ID to update quantity: ");
    scanf(" %d", &targetId); 

    int index = findProductIndex(inventory, numProducts, targetId);

    if (index != -1) {
        printf("Enter new Quantity: ");
        scanf(" %d", &newQuantity); 
        inventory[index].quantity = newQuantity;
        printf("Quantity updated successfully!\n");
    } else {
        printf("Product with ID %d not found.\n", targetId);
    }
}

void searchById(Product *inventory, int numProducts) {
    int targetId;
    printf("\nEnter Product ID to search: ");
    scanf(" %d", &targetId); 

    int index = findProductIndex(inventory, numProducts, targetId);

    if (index != -1) {
        printf("Product Found: ");
        printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",
               inventory[index].id,
               inventory[index].name,
               inventory[index].price,
               inventory[index].quantity);
    } else {
        printf("Product with ID %d not found.\n", targetId);
    }
}

void searchByName(Product *inventory, int numProducts) {
    char searchName[MAX_NAME_LENGTH + 1];
    printf("\nEnter name to search (partial allowed): ");
    scanf(" %50[^\n]", searchName); 

    int found = 0;
    for (int i = 0; i < numProducts; i++) {
        
        if (strstr(inventory[i].name, searchName) != NULL) {
            if (found == 0) {
                printf("Products Found:\n");
            }
            printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",
                   inventory[i].id,
                   inventory[i].name,
                   inventory[i].price,
                   inventory[i].quantity);
            found = 1;
        }
    }

    if (!found) {
        printf("No products found matching '%s'.\n", searchName);
    }
}

void searchByPriceRange(Product *inventory, int numProducts) {
    float minPrice, maxPrice;
    printf("\nEnter minimum price: ");
    scanf(" %f", &minPrice); 

    printf("Enter maximum price: ");
    scanf(" %f", &maxPrice); 

    int found = 0;
    for (int i = 0; i < numProducts; i++) {
        if (inventory[i].price >= minPrice && inventory[i].price <= maxPrice) {
            if (found == 0) {
                printf("Products in price range:\n");
            }
            printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",
                   inventory[i].id,
                   inventory[i].name,
                   inventory[i].price,
                   inventory[i].quantity);
            found = 1;
        }
    }

    if (!found) {
        printf("No products found in the price range %.2f to %.2f.\n", minPrice, maxPrice);
    }
}

void deleteProduct(Product **inventory, int *numProducts) {
    int targetId;
    printf("\nEnter Product ID to delete: ");
    scanf(" %d", &targetId); 

    int index = findProductIndex(*inventory, *numProducts, targetId);

    if (index == -1) {
        printf("Product with ID %d not found.\n", targetId);
        return;
    }

    for (int i = index; i < *numProducts - 1; i++) {
        (*inventory)[i] = (*inventory)[i + 1];
    }

    int newCount = *numProducts - 1;

    if (newCount == 0) {
        free(*inventory);
        *inventory = NULL;
    } else {
        Product *temp = (Product *)realloc(*inventory, newCount * sizeof(Product));
        if (temp == NULL) {
            printf("Warning: Could not shrink memory, but product is deleted.\n");
        } else {
            *inventory = temp;
        }
    }

    *numProducts = newCount;
    printf("Product deleted successfully!\n");
}
