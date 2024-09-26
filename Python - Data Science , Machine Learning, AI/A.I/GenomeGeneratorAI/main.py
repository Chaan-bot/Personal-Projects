import creature
import matplotlib.pyplot as plt
import networkx as nx
import seaborn as sns
import numpy as np
import time
import random
import matplotlib
matplotlib.use('Qt5Agg')

# Console-Based Visualization
def print_expanded_links(exp_links):
    def print_link(link, level=0):
        print(f"{'  ' * level}Link Name: {link.name}, Parent: {link.parent_name}, Recur: {link.recur}")
    for link in exp_links:
        parent_level = link.name.count('0')  # Assumes '0' indicates hierarchy levels
        print_link(link, parent_level)

# Tree or Network Visualization (Updates same window)
def visualize_genome_network(exp_links, ax):
    ax.clear()  # Clear the current Axes
    G = nx.DiGraph()
    for link in exp_links:
        G.add_node(link.name)
        if link.parent_name != 'None':
            G.add_edge(link.parent_name, link.name)
    
    pos = nx.spring_layout(G)
    nx.draw(G, pos, with_labels=True, ax=ax, node_size=3000, node_color="skyblue", font_size=10, font_color="black", font_weight="bold", edge_color="gray")
    ax.set_title("Genome Structure Visualization (Tree/Network)")
    plt.draw()
    plt.pause(0.001)

# 3D Visualization (Updates same window)
def visualize_3d_links(exp_links, ax):
    ax.clear()  # Clear the current Axes
    for link in exp_links:
        ax.scatter(link.joint_origin_xyz_1, link.joint_origin_xyz_2, link.joint_origin_xyz_3, label=link.name)
    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_zlabel('Z')
    ax.set_title("3D Genome Visualization")
    plt.draw()
    plt.pause(0.001)

# Heatmap Visualization (Updates same window)
def visualize_genome_heatmap(dna, ax, fig):
    ax.clear()  # Clear the current axes
    fig.clear()  # Clear the figure (including colorbars)
    
    genome_array = np.array([gene for gene in dna])
    
    # Recreate ax after clearing the figure
    ax = fig.add_subplot(111) 
    
    # Plot heatmap without generating a new colorbar each time
    sns.heatmap(genome_array, cmap="YlGnBu", annot=False, ax=ax)
    
    ax.set_title("Genome Heatmap")
    
    # Update the figure and pause to allow dynamic updates
    plt.draw()
    plt.pause(0.001)

# Mutation logic: Mutates a random gene

def mutate_genome(c):
    # Pick a random gene to mutate
    random_gene_idx = random.randint(0, len(c.dna) - 1)
    
    # Decide how many properties of the gene to mutate (randomly between 1 and all properties)
    num_properties_to_mutate = random.randint(0, len(c.dna[random_gene_idx]))

    for _ in range(num_properties_to_mutate):
        # Pick a random property of the gene to mutate
        random_property_idx = random.randint(0, len(c.dna[random_gene_idx]) - 1)
        
        # Erratic mutation factor: occasionally apply large jumps
        if random.random() < 0.2:  # 20% chance of a large mutation
            mutation_factor = random.uniform(-1, 1)  # Large mutation
        else:
            mutation_factor = random.uniform(-0.5, 0.5)  # Small mutation

        # Apply the mutation to the chosen property of the gene
        c.dna[random_gene_idx][random_property_idx] += mutation_factor


# Interactive Menu
def menu():
    print("Choose a visualization method:")
    print("1: Console-Based Visualization")
    print("2: Tree/Network Visualization")
    print("3: 3D Visualization")
    print("4: Heatmap Visualization")
    
    choice = input("Enter the number of your choice: ")
    return choice

# Main Function
if __name__ == "__main__":
    gene_count = 10
    iterations = 50  # Number of iterations to mutate and visualize
    delay = 0.5  # Time delay between iterations (in seconds)

    # Create a Creature
    c = creature.Creature(gene_count)
    
    # Show the menu
    choice = menu()

    if choice == "1":
        for i in range(iterations):
            print("\nConsole-Based Visualization (Iteration {}):".format(i + 1))
            mutate_genome(c)  # Mutate the genome
            exp_links = c.get_expanded_links()
            print_expanded_links(exp_links)
            time.sleep(delay)

    elif choice == "2":
        # Set up Matplotlib figure and axes for tree/network visualization
        plt.ion()  # Enable interactive mode
        fig, ax = plt.subplots()
        for i in range(iterations):
            mutate_genome(c)  # Mutate the genome
            exp_links = c.get_expanded_links()
            visualize_genome_network(exp_links, ax)  # Update the network visualization
            time.sleep(delay)
        plt.ioff()

    elif choice == "3":
        # Set up Matplotlib figure and axes for 3D visualization
        plt.ion()
        fig = plt.figure()
        ax = fig.add_subplot(111, projection='3d')
        for i in range(iterations):
            mutate_genome(c)  # Mutate the genome
            exp_links = c.get_expanded_links()
            visualize_3d_links(exp_links, ax)  # Update the 3D visualization
            time.sleep(delay)
        plt.ioff()

    elif choice == "4":
        # Set up Matplotlib figure and axes for heatmap visualization
        plt.ion()
        fig, ax = plt.subplots()
        for i in range(iterations):
            mutate_genome(c)  # Mutate the genome
            visualize_genome_heatmap(c.dna, ax, fig)  # Update the heatmap
            time.sleep(delay)
        plt.ioff()

    else:
        print("Invalid choice. Please choose a valid option.")
