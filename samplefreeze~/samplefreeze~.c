#include <m_pd.h>
#include <stdbool.h>

#define NAME "samplefreeze~"

// 'p' means 'patch' (don't wanna long names everywhere)
static t_class *p_class;

typedef struct _p {
	t_object x_obj;
	t_sample f; // plug for default inlet

	bool     is_sample_taken;
	t_sample frozen;

	t_inlet  *in2; // unfreeze indicator
	t_outlet *out;
} t_p;

void* p_new()
{
	t_p *x = (t_p*) pd_new(p_class);
	x->in2 = inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_signal, &s_signal);
	x->out = outlet_new(&x->x_obj, &s_signal);
	x->is_sample_taken = false;
	return (void *) x;
}

void p_free(t_p *x)
{
	inlet_free(x->in2);
	outlet_free(x->out);
}

t_int* p_perform(t_int *w) {
	t_p *x          = (t_p*) w[1];
	t_sample *in1   = (t_sample*) w[2];
	t_sample *in2   = (t_sample*) w[3];
	t_sample *out   = (t_sample*) w[4];
	t_sample n      = (int) w[5];
	t_sample frozen = x->frozen;

	if (!x->is_sample_taken) {
		frozen = in1[0];
		x->is_sample_taken = true;
	}

	for (int i = 0; i < n; ++i) {
		if (in2[i] >= 1) frozen = in1[i];
		out[i] = frozen;
	}

	x->frozen = frozen;
	return (w + 6); // arguments count plus one
}

void p_dsp(t_p *x, t_signal **sp)
{
	dsp_add(
		p_perform, 5, x,
		sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, sp[0]->s_n
	);
}

void samplefreeze_tilde_setup(void)
{
	p_class = class_new(
		gensym(NAME),
		(t_newmethod) p_new,
		(t_method) p_free,
		sizeof(t_p),
		CLASS_DEFAULT,
		A_DEFFLOAT,
		0
	);

	class_addmethod(p_class, (t_method) p_dsp, gensym("dsp"), 0);
	CLASS_MAINSIGNALIN(p_class, t_p, f);
}
