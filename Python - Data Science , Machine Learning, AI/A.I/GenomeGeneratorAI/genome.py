import numpy as np
import copy

class Genome():
    @staticmethod
    def get_random_gene(length):
        gene = np.array([np.random.random() for i in range(length)])
        #print(gene)
        return gene
    
    @staticmethod
    def get_random_genome(gene_length,gene_count):
        genome = [Genome.get_random_gene(gene_length) for i in range(gene_count)]
        return genome
    
    @staticmethod
    def get_gene_spec():
        gene_spec = { 
            "link-shape":  {"scale" : 1} ,
            "link-length" : {"scale" : 1} ,
            "link-radius" : {"scale" : 1} ,
            "link-recurrence" : {"scale" : 4} ,
            "link-mass" : {"scale" : 1} ,
            "joint-type":  {"scale" : 1} ,
            "joint-parent":  {"scale" : 1} ,
            "joint-axis-xyz" : {"scale" : 1}, 
            "joint-origin-rpy-1":  {"scale" : np.pi*2}, 
            "joint-origin-rpy-2":  {"scale" : np.pi*2}, 
            "joint-origin-rpy-3" : {"scale" : np.pi*2},
            "joint-origin-xyz-1" : {"scale" : 1},
            "joint-origin-xyz-2" : {"scale" : 1}, 
            "joint-origin-xyz-3" : {"scale" : 1},
            "control-waveform" :  {"scale" : 1},
            "control-amp" : {"scale" : 0.25}, 
            "control-freq" :  {"scale" : 1} 
        }
        ind = 0
        for key in gene_spec.keys():
            gene_spec[key]["ind"]=ind
            ind = ind+1

        return gene_spec
    
    @staticmethod
    def expandLinks(parent_link ,uniq_parent_name , flat_links ,exp_links ):
        children = [l for l in flat_links if l.parent_name == parent_link.name]
        for c in children:
            for r in range(int(c.recur)):
                c_copy = copy.copy(c)
                c_copy.parent_name = uniq_parent_name
                uniq_name = c_copy.name + str(len(exp_links))
                c_copy.name = uniq_name
                exp_links.append(c_copy)
                Genome.expandLinks(c,uniq_name, flat_links, exp_links)
    
    @staticmethod
    def get_gene_dict(gene,spec):
        gdict = {}
        for key in spec:
            ind = spec[key]["ind"]
            scale = spec[key]["scale"]
            gdict[key] = gene[ind] * scale
        return gdict



    @staticmethod
    def get_genome_dict(genome,spec):
        gdicts = []
        for gene in genome:
            gdicts.append(Genome.get_gene_dict(gene,spec))
        return gdicts

    @staticmethod
    def genome_to_links(gdicts):
        link_ind = 0
        parent_names = [str(link_ind)]
        links = []
        for gdict in gdicts:
            link_name = str(link_ind)
            parent_ind = gdict["joint-parent"] * len(parent_names)
            parent_name = parent_names[int(parent_ind)]
            recur = gdict["link-recurrence"]
            link = URDFLink(name=link_name,
                            parent_name=parent_name,
                            recur=recur + 1 ,
                            link_length=gdict["link-length"],
                            link_radius=gdict["link-radius"] ,
                            link_mass=gdict["link-mass"] ,
                            joint_type=gdict["joint-type"]  ,
                            joint_parent =gdict["joint-parent"]  ,
                            joint_axis_xyz=gdict["joint-axis-xyz"] , 
                            joint_origin_rpy_1 =gdict["joint-origin-rpy-1"],
                            joint_origin_rpy_2=gdict["joint-origin-rpy-2"],
                            joint_origin_rpy_3=gdict["joint-origin-rpy-3"],
                            joint_origin_xyz_1=gdict["joint-origin-xyz-1"],
                            joint_origin_xyz_2=gdict["joint-origin-xyz-2"], 
                            joint_origin_xyz_3=gdict["joint-origin-xyz-3"],
                            control_waveform=gdict["control-waveform"],
                            control_amp=gdict["control-amp"], 
                            control_freq=gdict["control-freq"])
            links.append(link)
            if link_ind !=0:
                parent_names.append(link_name)
            link_ind = link_ind +1

        links[0].parent_name = "None"
        return links





class URDFLink:
    def __init__(self , name , parent_name , recur , 
                link_length=0.1,
                link_radius=0.1  ,
                link_mass=0.1 ,
                joint_type=0.1  ,
                joint_parent =0.1  ,
                joint_axis_xyz=0.1 , 
                joint_origin_rpy_1 =0.1,
                joint_origin_rpy_2=0.1,
                joint_origin_rpy_3=0.1,
                joint_origin_xyz_1=0.1,
                joint_origin_xyz_2=0.1, 
                joint_origin_xyz_3=0.1,
                control_waveform=0.1,
                control_amp=0.1, 
                control_freq=0.1):
        self.name = name
        self.parent_name = parent_name 
        self.recur = recur
        self.link_length = link_length
        self.link_radius =link_radius
        self.link_mass = link_mass
        self.joint_type = joint_type
        self.joint_parent=joint_parent
        self.joint_axis_xyz = joint_axis_xyz
        self.joint_origin_rpy_1 = joint_origin_rpy_1
        self.joint_origin_rpy_2 = joint_origin_rpy_2
        self.joint_origin_rpy_3 = joint_origin_rpy_3
        self.joint_origin_xyz_1 = joint_origin_xyz_1
        self.joint_origin_xyz_2 = joint_origin_xyz_2
        self.joint_origin_xyz_3 = joint_origin_xyz_3
        self.control_waveform = control_waveform
        self.control_amp = control_amp
        self.control_freq = control_freq

    def to_link_xml(self):
        return ""



