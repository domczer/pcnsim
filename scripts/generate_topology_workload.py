import click
from functions import *
from graphs import *
from datasets.statistics import *

@click.group(chain=True, invoke_without_command=True)
def cli():
    pass

@cli.command(name='genTopo', help='Generates a topology for the simulation')
@click.option('-t','--topology', default='scale-free', type=click.Choice (['scale-free','barabasi-albert','watts-strogatz'], case_sensitive=False), help='Topology used in the simulation')
@click.option('-n','--nodes', type=int, default=10, help='Number of nodes in the topology')
@click.option('--alpha', default=0.5, help='Alpha parameter for scale-free topology')
@click.option('--beta', default=0.00001, help='Beta parameter for scale-free topology')
@click.option('--gamma', default=0.49999, help='Gamma parameter for scale-free topology')
@click.option('-k', default=2, help='K parameter for Watts-Strogatz graph')
@click.option('-p', default=0.1, help='P parameter for Watts-Strogatz graph')
@click.option('-m', default=2, help='M parameter for Barabasi-Albert graph')
@click.option('--lightning', is_flag=True, help='Channel capacities are modeled following real-world lightning network channels')
def generate_topology (topology, nodes, alpha, beta, gamma, k, p, m, lightning):
    if (topology == 'scale-free'):
        print ('Setting topology to scale-free')
        print ('Setting n to ' + str(nodes))
        print ('Setting alpha to ' + str(alpha))
        print ('Setting beta to ' + str(beta))
        print ('Setting gamma to ' + str(gamma))
        graph = generate_scale_free(nodes, alpha, beta, gamma)
    if (topology == 'watts-strogatz'):
        print ('Setting topology to watts-strogatz')
        print ('Setting n to ' + str(nodes))
        print ('Setting k to ' + str(k))
        print ('Setting p to ' + str(p))
        graph = generate_watts_strogatz(nodes, k, p)
    if (topology == 'barabasi-albert'):
        print ('Setting topology to watts-strogatz')
        print ('Setting n to ' + str(nodes))
        print ('Setting m to ' + str(m))
        graph = generate_barabasi_albert(nodes, m) 

    # --- Define the victim with highest degree ---
    degrees = graph.degree()
    hub_node = max(degrees, key=lambda x: x[1])[0]  # node with max degree
    print(f"Hub node (highest degree): {hub_node}")
    # -------------------------------

    # --- Attacker node ---
    attacker_id = -1
    graph.add_node(attacker_id)
    print(f"Added attacker node: {attacker_id}")
    # ------------------------------

    # --- Connect attacker to hub and hub's largest neighbor ---
    # Find hub's neighbor with largest channel (by edge 'capacity' attribute if present, else by degree)
    neighbors = list(graph.neighbors(hub_node))
    max_neighbor = None
    max_capacity = -1

    for neighbor in neighbors:
        # Try to get 'capacity' attribute, else fallback to degree
        if graph.has_edge(hub_node, neighbor):
            capacity = graph[hub_node][neighbor].get('capacity', 1)
            if capacity > max_capacity:
                max_capacity = capacity
                max_neighbor = neighbor

    if max_neighbor is None and neighbors:
        max_neighbor = neighbors[0]  # fallback

    print(f"Connecting attacker to hub {hub_node} and hub's max neighbor {max_neighbor}")

    # Add edges from attacker to hub and to the selected neighbor
    graph.add_edge(attacker_id, hub_node, capacity=max_capacity)
    if max_neighbor is not None:
        graph.add_edge(attacker_id, max_neighbor, capacity=max_capacity)
    # ------------------------------

    graph = adjust_edges(graph)
    initialize_attributes(graph, lightning, True if topology == 'scale-free' else False)

@cli.command(name='genWork', help='Generates a payment workload for the simulation')
@click.option('--n_payments', default=1, help='Number of payments in th network simulation')
@click.option('--min_payment', default=0.1, help='Minimum value of a payment in the network')
@click.option('--max_payment', default=1, help='Maximum value of a payment in the network')
@click.option('--any_node', is_flag=True, help='Transactions are issued by any node in the network, not only end hosts')
@click.option('--credit_card', is_flag=True, help='Transactions are modeled following a credit card dataset')
@click.option('--e_commerce', is_flag=True, help='Transactions are modeled following a e-commerce dataset')
def generate_workload(n_payments, min_payment, max_payment, any_node, credit_card, e_commerce):
    if (credit_card and e_commerce):
        click.echo('credit_card and e_commerce can\'t be used together. Use -h option to see running options.')
        return
    graph = read_graph()
    end_hosts = get_end_hosts_list(graph, any_node)
    create_workload(n_payments, min_payment, max_payment, end_hosts, credit_card, e_commerce)

if __name__ == "__main__":
    cli()
