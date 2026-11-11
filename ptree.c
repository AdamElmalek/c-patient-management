#include "ptree.h"

void init_ptree(PTree* ptree) {
    ptree = NULL;
}

PTree* insert_patient(PTree* root, char* name, char* id, unsigned char allergies, Visit* visits) {
    if (root == NULL) {
        PTree* new_node = (PTree*)malloc(sizeof(PTree));
        if (new_node == NULL) return NULL;
        new_node->left = new_node->right = new_node->patient = NULL;
        Patient* new_patient = (Patient*)malloc(sizeof(Patient));
        if (new_patient == NULL) return NULL;
        //new_patient->Name = strdup(name);
        new_patient->Name = (char*)malloc(sizeof(char*) * strlen(name) + 1);
        strcpy2(new_patient->Name, strlen(name) + 1, name, strlen(name));
        //strcpy(new_patient->ID, id);
        strcpy2(new_patient->ID, strlen(id) + 1, id, strlen(id));
        new_patient->Allergies = allergies;
        new_patient->Visits = visits;
        new_patient->nVisits = 0;
        //printf("new patent %s", new_patient->ID);
        new_node->patient = new_patient;
        return new_node;
    }
    else {
        if (strcmp(id, root->patient->ID) < 0) {
            root->left = insert_patient(root->left, name, id, allergies, visits);
        }
        else {
            root->right = insert_patient(root->right, name, id, allergies, visits);
        }
    }
    return root;
}

Patient* search_patient(PTree* ptree, char* id) {
    if (ptree == NULL || strcmp(ptree->patient->ID, id) == 0) {
        return ptree ? ptree->patient : NULL;
    }
    if (strcmp(id, ptree->patient->ID) < 0) {
        return search_patient(ptree->left, id);
    }
    else {
        return search_patient(ptree->right, id);
    }
}

void display_patients(PTree* ptree) {
    print_patients(ptree);
}

void save_ptree(PTree* root, FILE* file) {
    if (root != NULL) {
        save_patients(root->left, file);

        // Debugging output
        printf("Patient Name: %s\n", root->patient->Name);
        printf("Patient ID: %s\n", root->patient->ID);
        printf("Patient Allergies: %u\n", (unsigned int)root->patient->Allergies);

        fprintf(file, "%s %s %u\n", root->patient->Name, root->patient->ID, (unsigned int)root->patient->Allergies);

        save_patients(root->right, file);
    }
}

void print_patients(PTree* root) {
    if (root != NULL) {
        print_patients(root->left);
        printf("\nPatient: %s, ID: %s, Allergies: ", root->patient->Name, root->patient->ID); print_allergies(root->patient->Allergies);
        print_visits(root->patient->Visits);
        print_patients(root->right);
    }
}

void save_patients(PTree* root, FILE* file) {
    
    static int counter = 0;
    if (root != NULL) {
        save_patients(root->left, file);
        fprintf(file, "%d.%s;%s;", ++counter, root->patient->Name, root->patient->ID);
        SaveAllergiesToFile(file, root->patient->Allergies);
        fprintf(file, "\r\n");

        Visit* visits = root->patient->Visits;
        while (visits != NULL) {
            saveVisitToFile(file, visits);
            visits = visits->next;
        }

        fprintf(file, "=========================\n");
        save_patients( root->right, file);
    }
}

void removePatientfromTree(PTree** root, char* id)
{
        if (*root == NULL) return;

        // Find the node to be deleted
        if (strcmp(id, (*root)->patient->ID) < 0) {
            removePatientfromTree(&(*root)->left, id);
            
        }
        else if (strcmp(id, (*root)->patient->ID) > 0) {
            removePatientfromTree(&(*root)->right, id);
        }
        else {
            printf("Deleting....\n");
            // Node to be deleted found
            PTree* temp;
            if ((*root)->left == NULL) { // Node with only right child or no child
                // printf("Node with only right child or no child\n");
                temp = (*root)->right;
                // printf("Node with only right child or no child\n");
                // deletePatient((*root)->tpatient); // Delete the patient data
                free(*root);
                *root = temp;
            }
            else if ((*root)->right == NULL) {
                // printf("Node with only left child or no child\n");
                temp = (*root)->left;
                // deletePatient((*root)->tpatient); // Delete the patient data
                free(*root);
                *root = temp;
            }
            else {
                // printf("Node with two children\n");
                // Node with two children: Get the inorder successor (smallest in the right subtree)
                PTree* successorParent = *root;
                PTree* successor = (*root)->right;
                while (successor->left != NULL) {
                    successorParent = successor;
                    successor = successor->left;
                }

                // Copy the inorder successor's content to this node
                (*root)->patient = successor->patient;

                // Delete the inorder successor
                if (successorParent != *root) {
                    successorParent->left = successor->right;
                }
                else {
                    (*root)->right = successor->right;
                }
                // deletePatient(successor->tpatient); // Delete the patient data
                free(successor);
            }
            printf("Removed\n");
        }
    
}


void deleteTree(PTree* root) {
    if (root != NULL) {
        deleteTree(root->left);
        deleteTree(root->right);
        freePatient(root->patient);
        //free(root);
    }
}

void printPatientsAssignedtoDoctor(PTree* root, Doc* doc)
{
    if (root != NULL) {
        printPatientsAssignedtoDoctor(root->left, doc);
        Visit* current = root->patient->Visits;
        while (current->next != NULL) current = current->next;
        if (strcmp(current->Doctor->nLicense, doc->nLicense) == 0)
        {
            printf("Patient: %s, ID: %s, Visit Date: ", root->patient->Name, root->patient->ID);
            print_date(current->tArrival);
        }
         
        printPatientsAssignedtoDoctor(root->right, doc);
    }
}
