import React, { useState, useEffect } from 'react';
import { Plus, Edit2, Trash2, Package, DollarSign, TrendingUp, TrendingDown } from 'lucide-react';

export default function InventoryManagement() {
  const [items, setItems] = useState([]);
  const [showForm, setShowForm] = useState(false);
  const [editingId, setEditingId] = useState(null);
  const [formData, setFormData] = useState({
    name: '',
    sku: '',
    category: '',
    stock: 0,
    minStock: 0,
    price: 0,
    cost: 0
  });
  const [filter, setFilter] = useState('all');

  const categories = ['Elektronik', 'Pakaian', 'Makanan', 'Minuman', 'Alat Tulis', 'Lainnya'];

  const handleSubmit = (e) => {
    e.preventDefault();
    
    if (editingId) {
      setItems(items.map(item => 
        item.id === editingId ? { ...formData, id: editingId } : item
      ));
      setEditingId(null);
    } else {
      const newItem = {
        ...formData,
        id: Date.now(),
        stock: Number(formData.stock),
        minStock: Number(formData.minStock),
        price: Number(formData.price),
        cost: Number(formData.cost)
      };
      setItems([...items, newItem]);
    }
    
    resetForm();
  };

  const resetForm = () => {
    setFormData({
      name: '',
      sku: '',
      category: '',
      stock: 0,
      minStock: 0,
      price: 0,
      cost: 0
    });
    setShowForm(false);
    setEditingId(null);
  };

  const handleEdit = (item) => {
    setFormData(item);
    setEditingId(item.id);
    setShowForm(true);
  };

  const handleDelete = (id) => {
    if (confirm('Apakah Anda yakin ingin menghapus barang ini?')) {
      setItems(items.filter(item => item.id !== id));
    }
  };

  const updateStock = (id, change) => {
    setItems(items.map(item => {
      if (item.id === id) {
        const newStock = Math.max(0, item.stock + change);
        return { ...item, stock: newStock };
      }
      return item;
    }));
  };

  const filteredItems = items.filter(item => {
    if (filter === 'low') return item.stock <= item.minStock;
    if (filter === 'out') return item.stock === 0;
    return true;
  });

  const totalValue = items.reduce((sum, item) => sum + (item.stock * item.price), 0);
  const totalCost = items.reduce((sum, item) => sum + (item.stock * item.cost), 0);
  const potentialProfit = totalValue - totalCost;
  const lowStockCount = items.filter(item => item.stock <= item.minStock && item.stock > 0).length;
  const outOfStockCount = items.filter(item => item.stock === 0).length;

  return (
    <div className="min-h-screen bg-gradient-to-br from-blue-50 to-indigo-100 p-6">
      <div className="max-w-7xl mx-auto">
        <div className="bg-white rounded-lg shadow-lg p-6 mb-6">
          <h1 className="text-3xl font-bold text-gray-800 mb-6 flex items-center gap-2">
            <Package className="text-indigo-600" />
            Sistem Pengelolaan Stok Barang
          </h1>

          {/* Statistics */}
          <div className="grid grid-cols-1 md:grid-cols-4 gap-4 mb-6">
            <div className="bg-gradient-to-br from-blue-500 to-blue-600 rounded-lg p-4 text-white">
              <div className="text-sm opacity-90">Total Barang</div>
              <div className="text-3xl font-bold">{items.length}</div>
            </div>
            <div className="bg-gradient-to-br from-green-500 to-green-600 rounded-lg p-4 text-white">
              <div className="text-sm opacity-90">Nilai Stok</div>
              <div className="text-2xl font-bold">Rp {totalValue.toLocaleString('id-ID')}</div>
            </div>
            <div className="bg-gradient-to-br from-yellow-500 to-yellow-600 rounded-lg p-4 text-white">
              <div className="text-sm opacity-90">Stok Rendah</div>
              <div className="text-3xl font-bold">{lowStockCount}</div>
            </div>
            <div className="bg-gradient-to-br from-red-500 to-red-600 rounded-lg p-4 text-white">
              <div className="text-sm opacity-90">Stok Habis</div>
              <div className="text-3xl font-bold">{outOfStockCount}</div>
            </div>
          </div>

          {/* Filter and Add Button */}
          <div className="flex flex-wrap gap-4 mb-6">
            <button
              onClick={() => setShowForm(!showForm)}
              className="bg-indigo-600 text-white px-4 py-2 rounded-lg hover:bg-indigo-700 flex items-center gap-2 transition"
            >
              <Plus size={20} />
              Tambah Barang
            </button>
            
            <div className="flex gap-2">
              <button
                onClick={() => setFilter('all')}
                className={`px-4 py-2 rounded-lg transition ${
                  filter === 'all' ? 'bg-indigo-600 text-white' : 'bg-gray-200 text-gray-700 hover:bg-gray-300'
                }`}
              >
                Semua
              </button>
              <button
                onClick={() => setFilter('low')}
                className={`px-4 py-2 rounded-lg transition ${
                  filter === 'low' ? 'bg-yellow-600 text-white' : 'bg-gray-200 text-gray-700 hover:bg-gray-300'
                }`}
              >
                Stok Rendah
              </button>
              <button
                onClick={() => setFilter('out')}
                className={`px-4 py-2 rounded-lg transition ${
                  filter === 'out' ? 'bg-red-600 text-white' : 'bg-gray-200 text-gray-700 hover:bg-gray-300'
                }`}
              >
                Stok Habis
              </button>
            </div>
          </div>

          {/* Form */}
          {showForm && (
            <div className="bg-gray-50 p-6 rounded-lg mb-6 border-2 border-indigo-200">
              <h2 className="text-xl font-semibold mb-4">
                {editingId ? 'Edit Barang' : 'Tambah Barang Baru'}
              </h2>
              <form onSubmit={handleSubmit} className="grid grid-cols-1 md:grid-cols-2 gap-4">
                <div>
                  <label className="block text-sm font-medium text-gray-700 mb-1">
                    Nama Barang *
                  </label>
                  <input
                    type="text"
                    required
                    value={formData.name}
                    onChange={(e) => setFormData({...formData, name: e.target.value})}
                    className="w-full px-3 py-2 border border-gray-300 rounded-lg focus:ring-2 focus:ring-indigo-500 focus:border-transparent"
                  />
                </div>
                
                <div>
                  <label className="block text-sm font-medium text-gray-700 mb-1">
                    SKU/Kode Barang *
                  </label>
                  <input
                    type="text"
                    required
                    value={formData.sku}
                    onChange={(e) => setFormData({...formData, sku: e.target.value})}
                    className="w-full px-3 py-2 border border-gray-300 rounded-lg focus:ring-2 focus:ring-indigo-500 focus:border-transparent"
                  />
                </div>

                <div>
                  <label className="block text-sm font-medium text-gray-700 mb-1">
                    Kategori *
                  </label>
                  <select
                    required
                    value={formData.category}
                    onChange={(e) => setFormData({...formData, category: e.target.value})}
                    className="w-full px-3 py-2 border border-gray-300 rounded-lg focus:ring-2 focus:ring-indigo-500 focus:border-transparent"
                  >
                    <option value="">Pilih Kategori</option>
                    {categories.map(cat => (
                      <option key={cat} value={cat}>{cat}</option>
                    ))}
                  </select>
                </div>

                <div>
                  <label className="block text-sm font-medium text-gray-700 mb-1">
                    Stok Awal *
                  </label>
                  <input
                    type="number"
                    required
                    min="0"
                    value={formData.stock}
                    onChange={(e) => setFormData({...formData, stock: e.target.value})}
                    className="w-full px-3 py-2 border border-gray-300 rounded-lg focus:ring-2 focus:ring-indigo-500 focus:border-transparent"
                  />
                </div>

                <div>
                  <label className="block text-sm font-medium text-gray-700 mb-1">
                    Stok Minimum *
                  </label>
                  <input
                    type="number"
                    required
                    min="0"
                    value={formData.minStock}
                    onChange={(e) => setFormData({...formData, minStock: e.target.value})}
                    className="w-full px-3 py-2 border border-gray-300 rounded-lg focus:ring-2 focus:ring-indigo-500 focus:border-transparent"
                  />
                </div>

                <div>
                  <label className="block text-sm font-medium text-gray-700 mb-1">
                    Harga Modal (Rp) *
                  </label>
                  <input
                    type="number"
                    required
                    min="0"
                    value={formData.cost}
                    onChange={(e) => setFormData({...formData, cost: e.target.value})}
                    className="w-full px-3 py-2 border border-gray-300 rounded-lg focus:ring-2 focus:ring-indigo-500 focus:border-transparent"
                  />
                </div>

                <div>
                  <label className="block text-sm font-medium text-gray-700 mb-1">
                    Harga Jual (Rp) *
                  </label>
                  <input
                    type="number"
                    required
                    min="0"
                    value={formData.price}
                    onChange={(e) => setFormData({...formData, price: e.target.value})}
                    className="w-full px-3 py-2 border border-gray-300 rounded-lg focus:ring-2 focus:ring-indigo-500 focus:border-transparent"
                  />
                </div>

                <div className="md:col-span-2 flex gap-3">
                  <button
                    type="submit"
                    className="bg-indigo-600 text-white px-6 py-2 rounded-lg hover:bg-indigo-700 transition"
                  >
                    {editingId ? 'Update' : 'Simpan'}
                  </button>
                  <button
                    type="button"
                    onClick={resetForm}
                    className="bg-gray-300 text-gray-700 px-6 py-2 rounded-lg hover:bg-gray-400 transition"
                  >
                    Batal
                  </button>
                </div>
              </form>
            </div>
          )}

          {/* Items List */}
          <div className="overflow-x-auto">
            {filteredItems.length === 0 ? (
              <div className="text-center py-12 text-gray-500">
                <Package size={48} className="mx-auto mb-4 opacity-50" />
                <p>Belum ada barang. Klik "Tambah Barang" untuk memulai.</p>
              </div>
            ) : (
              <table className="w-full">
                <thead className="bg-gray-100">
                  <tr>
                    <th className="px-4 py-3 text-left text-sm font-semibold text-gray-700">SKU</th>
                    <th className="px-4 py-3 text-left text-sm font-semibold text-gray-700">Nama Barang</th>
                    <th className="px-4 py-3 text-left text-sm font-semibold text-gray-700">Kategori</th>
                    <th className="px-4 py-3 text-center text-sm font-semibold text-gray-700">Stok</th>
                    <th className="px-4 py-3 text-right text-sm font-semibold text-gray-700">Harga Modal</th>
                    <th className="px-4 py-3 text-right text-sm font-semibold text-gray-700">Harga Jual</th>
                    <th className="px-4 py-3 text-right text-sm font-semibold text-gray-700">Profit/Unit</th>
                    <th className="px-4 py-3 text-center text-sm font-semibold text-gray-700">Aksi</th>
                  </tr>
                </thead>
                <tbody className="divide-y divide-gray-200">
                  {filteredItems.map(item => {
                    const profit = item.price - item.cost;
                    const profitMargin = ((profit / item.cost) * 100).toFixed(1);
                    const isLowStock = item.stock <= item.minStock && item.stock > 0;
                    const isOutOfStock = item.stock === 0;
                    
                    return (
                      <tr key={item.id} className={`hover:bg-gray-50 ${
                        isOutOfStock ? 'bg-red-50' : isLowStock ? 'bg-yellow-50' : ''
                      }`}>
                        <td className="px-4 py-3 text-sm text-gray-700">{item.sku}</td>
                        <td className="px-4 py-3 text-sm font-medium text-gray-900">{item.name}</td>
                        <td className="px-4 py-3 text-sm text-gray-600">{item.category}</td>
                        <td className="px-4 py-3 text-center">
                          <div className="flex items-center justify-center gap-2">
                            <button
                              onClick={() => updateStock(item.id, -1)}
                              className="bg-red-500 text-white w-6 h-6 rounded hover:bg-red-600 flex items-center justify-center"
                            >
                              -
                            </button>
                            <span className={`font-semibold min-w-12 ${
                              isOutOfStock ? 'text-red-600' : isLowStock ? 'text-yellow-600' : 'text-gray-900'
                            }`}>
                              {item.stock}
                            </span>
                            <button
                              onClick={() => updateStock(item.id, 1)}
                              className="bg-green-500 text-white w-6 h-6 rounded hover:bg-green-600 flex items-center justify-center"
                            >
                              +
                            </button>
                          </div>
                          {isLowStock && <div className="text-xs text-yellow-600 mt-1">Min: {item.minStock}</div>}
                          {isOutOfStock && <div className="text-xs text-red-600 mt-1">Habis!</div>}
                        </td>
                        <td className="px-4 py-3 text-sm text-right text-gray-700">
                          Rp {item.cost.toLocaleString('id-ID')}
                        </td>
                        <td className="px-4 py-3 text-sm text-right font-medium text-gray-900">
                          Rp {item.price.toLocaleString('id-ID')}
                        </td>
                        <td className="px-4 py-3 text-sm text-right">
                          <div className={`font-medium ${profit >= 0 ? 'text-green-600' : 'text-red-600'}`}>
                            Rp {profit.toLocaleString('id-ID')}
                          </div>
                          <div className="text-xs text-gray-500">({profitMargin}%)</div>
                        </td>
                        <td className="px-4 py-3">
                          <div className="flex justify-center gap-2">
                            <button
                              onClick={() => handleEdit(item)}
                              className="text-blue-600 hover:text-blue-800"
                            >
                              <Edit2 size={18} />
                            </button>
                            <button
                              onClick={() => handleDelete(item.id)}
                              className="text-red-600 hover:text-red-800"
                            >
                              <Trash2 size={18} />
                            </button>
                          </div>
                        </td>
                      </tr>
                    );
                  })}
                </tbody>
              </table>
            )}
          </div>
        </div>
      </div>
    </div>
  );
}
